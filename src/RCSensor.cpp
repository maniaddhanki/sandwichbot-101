#include "RCSensor.h"
#include "Utils.h"
#include <Arduino.h>
#define SENSOR_COUNT 11

RCSensor::RCSensor(const int line_sensors_pin[], boolean isVirtual) {
  _maxValue = 1500;
  _isVirtual = isVirtual;
  for (uint8_t index = 0; index < SENSOR_COUNT; index++) {
    _sensorPins[index] = line_sensors_pin[index];
    _maxValueCalibrated[index] = 0;
    _minValueCalibrated[index] = 1500;
  }
}

boolean readSensorsFromSerial(uint16_t sensorValues[]) {
  delay(6);
  boolean newSensorVailable = false;
  char serialCharecters[] = {'L', 'l', 'c', 'r', 'R'};
  while (Serial1.available()) {
    char inChar = Utils::readNextCharacterFromSerial1();
    int serialCounter = 0;
    if (inChar == 'S') {
      newSensorVailable = true;
      inChar = Utils::readNextCharacterFromSerial1();
      while (inChar == serialCharecters[serialCounter] && serialCounter < 5) {
        sensorValues[serialCounter] = Utils::readNextByteFromSerial1();
        inChar = Utils::readNextCharacterFromSerial1();
        Serial.print(sensorValues[serialCounter]);
        Serial.print(" ");
        serialCounter++;
      }
    }
  }
  return newSensorVailable;
}
boolean RCSensor::readSensors(uint16_t sensorValues[]) {
  if (_isVirtual) {
    return readSensorsFromSerial(sensorValues);
  }
  for (uint8_t index = 0; index < SENSOR_COUNT; index++) {
    sensorValues[index] = _maxValue;
    // make sensor line an output (drives low briefly, but doesn't matter)
    pinMode(_sensorPins[index], OUTPUT);
    // drive sensor line high
    digitalWrite(_sensorPins[index], HIGH);
  }
  delayMicroseconds(10); // charge lines for 10 us

  {
    // disable interrupts so we can switch all the pins as close to the same
    // time as possible
    noInterrupts();

    // record start time before the first sensor is switched to input
    // (similarly, time is checked before the first sensor is read in the
    // loop below)
    uint32_t startTime = micros();
    uint16_t time = 0;

    for (uint8_t index = 0; index < SENSOR_COUNT; index++) {
      // make sensor line an input (should also ensure pull-up is disabled)
      pinMode(_sensorPins[index], INPUT);
    }

    interrupts(); // re-enable

    while (time < _maxValue) {
      // disable interrupts so we can read all the pins as close to the same
      // time as possible
      noInterrupts();

      time = micros() - startTime;
      for (uint8_t i = 0; i < SENSOR_COUNT; i++) {
        if ((digitalRead(_sensorPins[i]) == LOW) && (time < sensorValues[i])) {
          // record the first time the line reads low
          sensorValues[i] = time;
        }
      }

      interrupts(); // re-enable
    }
  }
  return true;
}
boolean RCSensor::readCalibrateSensors(uint16_t sensorValues[]) {
  boolean readState = readSensors(sensorValues);
  for (uint8_t index = 0; index < SENSOR_COUNT; index++) {
    uint16_t calmin = _minValueCalibrated[index];
    uint16_t calmax = _maxValueCalibrated[index];
    uint16_t denominator = calmax - calmin;
    int32_t value = 0;
    if (denominator != 0) {
      value = (((int32_t)sensorValues[index]) - calmin) * 1000 / denominator;
    }
    if (value < 0) {
      value = 0;
    } else if (value > 1000) {
      value = 1000;
    }
    sensorValues[index] = value;
  }
  return readState;
}

void RCSensor::resetCalibrateSensors() {
  for (uint8_t index = 0; index < SENSOR_COUNT; index++) {
    this->_maxValueCalibrated[index] = 0;
    this->_minValueCalibrated[index] = _maxValue;
  }
}

void RCSensor::calibrateSensors(int numberOfReadings) {
  uint16_t sensorValues[SENSOR_COUNT] = {0};
  for (int count = 0; count < numberOfReadings; count++) {
    readSensors(sensorValues);
    for (uint8_t index = 0; index < SENSOR_COUNT; index++) {
      if (_maxValueCalibrated[index] < sensorValues[index]) {
        _maxValueCalibrated[index] = sensorValues[index];
      }
      if (_minValueCalibrated[index] > sensorValues[index]) {
        _minValueCalibrated[index] = sensorValues[index];
      }
    }
  }
}

void RCSensor::readCalibratedMin(uint16_t minValues[]) {
  for (uint8_t index = 0; index < 11; index++) {
    minValues[index] = _minValueCalibrated[index];
  }
}

void RCSensor::readCalibratedMax(uint16_t maxValues[]) {
  for (uint8_t index = 0; index < 11; index++) {
    maxValues[index] = _maxValueCalibrated[index];
  }
}

void RCSensor::setCalibratedMax(uint16_t maxValues[]) {
  for (uint8_t index = 0; index < 11; index++) {
    _maxValueCalibrated[index] = maxValues[index];
  }
}

void RCSensor::setCalibratedmin(uint16_t minValues[]) {
  for (uint8_t index = 0; index < 11; index++) {
    _minValueCalibrated[index] = minValues[index];
  }
}

uint16_t RCSensor::readLinePrivate(uint16_t sensorValues[],
                                   bool invertReadings) {
  bool onLine = false;
  uint32_t avg = 0; // this is for the weighted total
  uint16_t sum = 0; // this is for the denominator, which is <= 64000

  for (uint8_t i = 0; i < 5; i++) {
    uint16_t value = sensorValues[i];
    if (invertReadings) {
      value = 1000 - value;
    }

    // keep track of whether we see the line at all
    if (value > 200) {
      onLine = true;
    }

    // only average in values that are above a noise threshold
    if (value > 50) {
      avg += (uint32_t)value * (i * 1000);
      sum += value;
    }
  }

  if (!onLine) {
    // If it last read to the left of center, return 0.
    if (_lastPosition < (_sensorCount - 1) * 1000 / 2) {
      return 0;
    }
    // If it last read to the right of center, return the max.
    else {
      return (_sensorCount - 1) * 1000;
    }
  }

  _lastPosition = avg / sum;
  return _lastPosition;
}
int RCSensor::isOnlane(uint16_t sensorValues[], int threshold) {
  if (sensorValues[0] > threshold || sensorValues[1] > threshold ||
      sensorValues[2] > threshold || sensorValues[3] > threshold ||
      sensorValues[4] > threshold) {
    return 1;
  }
  return 0;
}
int RCSensor::isOnline(uint16_t sensorValues[], int threshold) {
  if (sensorValues[1] > threshold || sensorValues[2] > threshold ||
      sensorValues[3] > threshold) {
    if (sensorValues[0] > threshold && sensorValues[2] > threshold &&
        sensorValues[4] > threshold) {
      return 2;
    } else {
      return 1;
    }
  }
  return 0;
}

int RCSensor::isSensorOnline(uint16_t sensorValues[], int threshold,
                             int index) {
  if (sensorValues[index] > threshold) {
    return 1;
  }
  return 0;
}

void RCSensor::printSensorArray(uint16_t sensors[SENSOR_COUNT]) {
  for (int sensorIndex = 0; sensorIndex < SENSOR_COUNT; sensorIndex++) {
    Serial.print(sensors[sensorIndex]);
    Serial.print(" ");
  }
  Serial.println(" ");
}

bool RCSensor::hasSpaceToMove(uint16_t sensors[SENSOR_COUNT], int threshold) {
  return sensors[7] > threshold && sensors[8] > threshold;
}

void RCSensor::loadCalibratedLineSensors(uint16_t lineSensorMaxCalib[5],
                                         uint16_t lineSensorMinCalib[5]) {
  for (int index = 0; index < 5; index++) {
    _maxValueCalibrated[index] = lineSensorMaxCalib[index];
    _minValueCalibrated[index] = lineSensorMinCalib[index];
  }
}

void RCSensor::loadCalibratedSideSensors(uint16_t sideSensorMaxCalib[6],
                                         uint16_t sideSensorMinCalib[6]) {
  for (int index = 0; index < 6; index++) {
    _maxValueCalibrated[5 + index] = sideSensorMaxCalib[index];
    _minValueCalibrated[5 + index] = sideSensorMinCalib[index];
  }
}