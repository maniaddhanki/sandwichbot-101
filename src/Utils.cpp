#include <Arduino.h>
#include "Utils.h"
#include "main.h"

char Utils::readNextCharacterFromSerial1()
{
    while (Serial1.available() == 0)
    {
    }
    return (char)Serial1.read();
}

byte Utils::readNextByteFromSerial1()
{
    while (Serial1.available() == 0)
    {
    }
    return Serial1.read();
}

void Utils::readNBytesFromSerial1(uint8_t noOfBytes, uint8_t byteArray[])
{
    for (uint8_t index = 0; index < noOfBytes; index++)
    {
        byteArray[index] = Utils::readNextByteFromSerial1();
    }
}

void Utils::copyByteArrayElements(uint8_t array1[], uint8_t array2[], uint8_t numberOfElements)
{
    for (uint8_t index = 0; index < numberOfElements; index++)
    {
        array1[index] = array2[index];
    }
}