#include <Arduino.h>
#include "LED.h"
int TIME_SCALE_FACTOR = 8;
unsigned int LED::_ledPins[3];
unsigned int LED::_blinkTime[3];
volatile boolean LED::_isBlinking[3];
int LED::counter; 
void LED::init(int pin1, int pin2, int pin3)
{
  _ledPins[0] = pin1;
  _ledPins[1] = pin2;
  _ledPins[2] = pin3;
  for (int index = 0; index < 3; index++)
  {
    _blinkTime[index] = 0;
    _isBlinking[index] = false;
    pinMode(_ledPins[index], OUTPUT);
    digitalWrite(_ledPins[index], HIGH);
  }
  delay(1000);
  for (int index = 0; index < 3; index++)
  {
    digitalWrite(_ledPins[index], LOW);
  }
  LED::updateTimerInterrupt();
}

void LED::setLedOn(int ledNum)
{
  digitalWrite(_ledPins[ledNum - 1], HIGH);
  _isBlinking[ledNum - 1] = false;
  LED::updateTimerInterrupt();
}
void LED::setLedState(int state1,int state2,int state3){
  digitalWrite(_ledPins[0], state1);
  digitalWrite(_ledPins[1], state2);
  digitalWrite(_ledPins[2], state3);
  for (int index = 0; index < 3; index++)
  {
    _blinkTime[index] = 0;
    _isBlinking[index] = false;
  }
  LED::updateTimerInterrupt();
}
void LED::updateTimerInterrupt()
{
  if (isLedBlinking())
  {
    LED::setupTimmer5ForInterrupts();
  }
  else
  {
    LED::stopTimmer5Interrupts();
  }
}
boolean LED::isLedBlinking()
{
  boolean isBlinking = false;
  for (int index = 0; index < 3; index++)
  {
    isBlinking |= _isBlinking[index];
  }
  return isBlinking;
}
void LED::setLedOff(int ledNum)
{
  digitalWrite(_ledPins[ledNum - 1], LOW);
  _isBlinking[ledNum - 1] = false;
  LED::updateTimerInterrupt();
}

void LED::setLedBlink(int ledNum, int interval)
{
  _isBlinking[ledNum - 1] = true;
  _blinkTime[ledNum - 1] = interval;
  digitalWrite(_ledPins[ledNum - 1], HIGH);
  LED::updateTimerInterrupt();
}

void LED::updateLedStates()
{
  for (int index = 0; index < 3; index++)
  {
    if (_isBlinking[index] && (counter % _blinkTime[index] == 0))
    {
      digitalWrite(_ledPins[index], !digitalRead(_ledPins[index]));
    }
  }
  counter++;
}

ISR(TIMER5_COMPA_vect)
{
  LED::updateLedStates();
}

void LED::stopTimmer5Interrupts()
{
  cli();      //stop interrupts
  TCCR5A = 0; // set entire TCCR1A register to 0
  TCCR5B = 0; // same for TCCR1B
  TCNT5 = 0;  //initialize counter value to 0
  TIMSK5 = 0;
  sei();
}
void LED::setupTimmer5ForInterrupts()
{
  // initialize timer1 interupt at every 1000micro sec or 1 milli sec
  //set timer1 interrupt at 1000Hz
  cli();      //stop interrupts
  TCCR5A = 0; // set entire TCCR1A register to 0
  TCCR5B = 0; // same for TCCR1B
  TCNT5 = 0;  //initialize counter value to 0
  // set compare match register for 1000hz increments
  // calculation for 1024 prescalar 15.625-1
  // @ 256 62.5-1
  //@64 250-1
  //16000000/1024/TIME_SCALE_FACTOR -1
  OCR5A = ((16000 / TIME_SCALE_FACTOR) - 1);

  // turn on CTC mode
  TCCR5B |= (1 << WGM52);
  // Set CS10 and CS11 bits for 1 prescaler
  TCCR5B |= (1 << CS50);
  TCCR5B |= (1 << CS52);
  // enable timer compare interrupt
  TIMSK5 |= (1 << OCIE5A);
  sei();
}