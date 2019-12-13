#ifndef SRC_BATTERY_H_
#define SRC_BATTERY_H_
#include <Arduino.h>

class Battery
{
private:
    static int pin_battery;

public:
    static int getBatteryVoltage();
};

#endif /* SRC_BATTERY_H_ */