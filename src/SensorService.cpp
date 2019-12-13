#include <Arduino.h>
#include "SensorService.h"
#include "main.h"
#include "Car.h"
int sideSensorMaxCalib[5][6] = {{1500, 1500, 1500, 1500, 1500, 1500},
                                {1500, 1500, 1500, 1500, 1500, 1500},
                                {1500, 1500, 1500, 1500, 1500, 1500},
                                {1500, 1500, 1500, 1500, 1500, 1500},
                                {1500, 1500, 1500, 1500, 1500, 1500}};
int sideSensorMinCalib[5][6] = {{0, 0, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0, 0}};
int lineSensorMaxCalib[5][5] = {{1500, 1500, 1500, 1500, 1500},
                                {1500, 1500, 1500, 1500, 1500},
                                {1500, 1500, 1500, 1500, 1500},
                                {1500, 1500, 1500, 1500, 1500},
                                {1500, 1500, 1500, 1500, 1500}};
int lineSensorMinCalib[5][5] = {{240, 240, 240, 240, 240},
                                {240, 240, 240, 240, 240},
                                {240, 240, 240, 240, 240},
                                {240, 240, 240, 240, 240},
                                {240, 240, 240, 240, 240}};

SensorService::SensorService()
{
}
void SensorService::printMaxAndMinCalibration(RCSensor *rcsensor)
{
  uint16_t sensorValues[11] = {0};
  rcsensor->readCalibratedMin(sensorValues);
  for (uint8_t i = 0; i < 11; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print(' ');
  }
  Serial.println();
  rcsensor->readCalibratedMax(sensorValues);
  for (uint8_t i = 0; i < 11; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print(' ');
  }
  Serial.println();
}
void SensorService::calibrateSideSensors(RCSensor *rcsensor)
{
  uint16_t maxcalib[11] = {0};
  uint16_t mincalib[11] = {0};
  rcsensor->readCalibratedMin(mincalib);
  rcsensor->readCalibratedMax(maxcalib);
  for (int index = 0; index < 6; index++)
  {
    maxcalib[5 + index] = sideSensorMaxCalib[BOT_ID][index];
    mincalib[5 + index] = sideSensorMinCalib[BOT_ID][index];
  }
  rcsensor->setCalibratedmin(mincalib);
  rcsensor->setCalibratedMax(maxcalib);
  SensorService::printMaxAndMinCalibration(rcsensor);
}
void SensorService::printCalibrateSensor(RCSensor *rcsensor)
{
  uint16_t sensorsAccum[11] = {0};
  uint16_t sensors[11] = {0};
  for (int index = 0; index < 5; index++)
  {
    rcsensor->readCalibrateSensors(sensors);
    for (int sensorIndex = 0; sensorIndex < 11; sensorIndex++)
    {
      sensorsAccum[sensorIndex] += sensors[sensorIndex];
    }
  }
  for (int sensorIndex = 0; sensorIndex < 11; sensorIndex++)
  {
    sensorsAccum[sensorIndex] = sensorsAccum[sensorIndex] / 5;
    Serial.print(sensorsAccum[sensorIndex]);
    Serial.print(" ");
  }
  Serial.println(" ");
}
void SensorService::printSensorsContinously(RCSensor *rcsensor)
{
  calibrateSideSensors(rcsensor);
  while (1)
  {
    delay(500);
    printSensor(rcsensor);
    printCalibrateSensor(rcsensor);
  }
}
void SensorService::printSensor(RCSensor *rcsensor)
{
  uint16_t sensorsAccum[11] = {0};
  uint16_t sensors[11] = {0};
  for (int index = 0; index < 5; index++)
  {
    rcsensor->readSensors(sensors);
    for (int sensorIndex = 0; sensorIndex < 11; sensorIndex++)
    {
      sensorsAccum[sensorIndex] += sensors[sensorIndex];
    }
  }
  for (int sensorIndex = 0; sensorIndex < 11; sensorIndex++)
  {
    sensorsAccum[sensorIndex] = sensorsAccum[sensorIndex] / 5;
    Serial.print(sensorsAccum[sensorIndex]);
    Serial.print(" ");
  }
  Serial.println(" ");
}
void SensorService::printSensorArray(uint16_t sensors[11])
{
  for (int sensorIndex = 0; sensorIndex < 11; sensorIndex++)
  {
    Serial.print(sensors[sensorIndex]);
    Serial.print(" ");
  }
  Serial.println(" ");
}
void SensorService::calibrateLineSensors(RCSensor *rcsensor, Car *car)
{
  uint16_t sensorValues[11] = {0};
  for (uint8_t i = 0; i < 11; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print(' ');
  }
  Serial.println("startCalibration");
  rcsensor->resetCalibrateSensors();
  car->setSpeeds(50, -50);
  for (int i = 0; i < 50; i++)
  {
    rcsensor->calibrateSensors(20);
  }
  car->setSpeeds(-50, 50);
  for (int i = 0; i < 50; i++)
  {
    rcsensor->calibrateSensors(20);
  }
  car->setSpeeds(0, 0);
  SensorService::printMaxAndMinCalibration(rcsensor);
}

void SensorService::loadCalibratedLineSensors(RCSensor *rcsensor)
{
  rcsensor->loadCalibratedLineSensors(lineSensorMaxCalib[BOT_ID], lineSensorMinCalib[BOT_ID]);
}

void SensorService::loadCalibratedSideSensors(RCSensor *rcsensor)
{
  rcsensor->loadCalibratedSideSensors(sideSensorMaxCalib[BOT_ID], sideSensorMinCalib[BOT_ID]);
}