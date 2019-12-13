#include <Arduino.h>
#include "Battery.h"
#include "main.h"

int Battery::pin_battery = A9;

int Battery::getBatteryVoltage()
{
    int voltage = 0;
    for (uint8_t i = 0; i < 10; i++)
    {
        voltage += analogRead(pin_battery);
    }
    return voltage / 80;
}
