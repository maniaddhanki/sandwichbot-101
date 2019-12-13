#include <Arduino.h>
#include "Buttons.h"

unsigned int Buttons::_btPins[3];
void Buttons::init(int pin1, int pin2, int pin3)
{
    _btPins[0] = pin1;
    _btPins[1] = pin2;
    _btPins[2] = pin3;
    for (int index = 0; index < 3; index++)
    {
        pinMode(_btPins[index], INPUT_PULLUP);
    }
}
int Buttons::getNextButtonPressed()
{
    delay(500);
    while (!(digitalRead(_btPins[0]) && digitalRead(_btPins[1]) && digitalRead(_btPins[2])))
    {
    }
    while (1)
    {
        delay(50);
        for (int index = 0; index < 3; index++)
        {
            if (digitalRead(_btPins[index]) < HIGH)
            {
                return index + 1;
            }
        }
    }
}