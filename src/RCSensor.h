#ifndef SRC_RCSENSOR_H_
#define SRC_RCSENSOR_H_
#include <Arduino.h>
class RCSensor
{
private:
  unsigned int _sensorPins[11];
  uint32_t _maxValue;
  uint16_t _maxValueCalibrated[11];
  uint16_t _minValueCalibrated[11];
  uint8_t _sensorCount = 5;
  uint16_t _lastPosition = 0;
  boolean _isVirtual = false;

public:
  RCSensor(const int line_sensors_pin[], boolean isVirtual);
  boolean readSensors(uint16_t sensorValues[]);
  boolean readCalibrateSensors(uint16_t sensorValues[]);
  void readCalibratedMin(uint16_t minValues[]);
  void readCalibratedMax(uint16_t maxValues[]);
  void setCalibratedMax(uint16_t maxValues[]);
  void setCalibratedmin(uint16_t minValues[]);
  void calibrateSensors(int numberOfReadings);
  void resetCalibrateSensors();
  uint16_t readLinePrivate(uint16_t sensorValues[], bool invertReadings);
  int isOnlane(uint16_t sensorValues[], int thresold);
  int isOnline(uint16_t sensorValues[], int thresold);
  int isSensorOnline(uint16_t sensorValues[], int thresold, int index);
  void static printSensorArray(uint16_t sensors[11]);
  bool static hasSpaceToMove(uint16_t sensors[11], int threshold);
  void loadCalibratedLineSensors(uint16_t lineSensorMaxCalib[5], uint16_t lineSensorMinCalib[5]);
  void loadCalibratedSideSensors(uint16_t lineSensorMaxCalib[5], uint16_t lineSensorMinCalib[5]);
};
#endif /* SRC_RCSENSOR_H_ */