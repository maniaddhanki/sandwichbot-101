#ifndef SRC_SENSORSERVICE_H_
#define SRC_SENSORSERVICE_H_
#include <Arduino.h>
#include "RCSensor.h"
#include "Car.h"
class SensorService
{
  private:
  void blinkServiceTest();
  
  void printSensor(RCSensor *rcsensor);
  public:
    SensorService();
    void calibrateSideSensors(RCSensor *rcsensor);
    void printSensorsContinously(RCSensor *rcsensor);
    void calibrateLineSensors(RCSensor *rcsensor,Car *car);
    void static printSensorArray(uint16_t sensors[11]);
    void static printCalibrateSensor(RCSensor *rcsensor);
    void static printMaxAndMinCalibration(RCSensor *rcsensor);
    void loadCalibratedLineSensors(RCSensor *rcsensor);
    void loadCalibratedSideSensors(RCSensor *rcsensor);
};
#endif /* SRC_SENSORSERVICE_H_ */
