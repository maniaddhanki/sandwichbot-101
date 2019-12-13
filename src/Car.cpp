#include <Arduino.h>
#include "Car.h"

Car::Car(int pin_m1_a, int pin_m1_b, int pwm_m1, int pin_m2_a, int pin_m2_b, int pwm_m2, int standby, boolean isVirtual)
{
  this->pin_m1_a = pin_m1_a;
  this->pin_m1_b = pin_m1_b;
  this->pwm_m1 = pwm_m1;
  this->pin_m2_a = pin_m2_a;
  this->pin_m2_b = pin_m2_b;
  this->pwm_m2 = pwm_m2;
  this->standby = standby;
  this->isVirtual = isVirtual;
  pinMode(pin_m1_a, OUTPUT);
  pinMode(pin_m1_b, OUTPUT);
  pinMode(pin_m2_a, OUTPUT);
  pinMode(pin_m2_b, OUTPUT);
  pinMode(pwm_m1, OUTPUT);
  pinMode(pwm_m2, OUTPUT);
  pinMode(standby, OUTPUT);
  digitalWrite(pin_m1_a, LOW);
  digitalWrite(pin_m1_b, LOW);
  digitalWrite(pin_m2_a, LOW);
  digitalWrite(pin_m2_b, LOW);
  digitalWrite(pwm_m1, LOW);
  digitalWrite(pwm_m2, LOW);
  digitalWrite(standby, LOW);
  this->_lastM1Speed = 0;
  this->_lastM2Speed = 0;
}
void Car::setSpeeds(int m1Speed, int m2Speed)
{
  if (isVirtual)
  {
    Serial1.print(m2Speed);
    Serial1.print(",");
    Serial1.println(m1Speed);
  }
  setM1Speed(m1Speed);
  setM2Speed(m2Speed);
}
void Car::setSpeeds(int m1Speed, int m2Speed, int speedStep)
{
  if (isVirtual)
  {
    Serial1.print(m2Speed);
    Serial1.print(",");
    Serial1.println(m1Speed);
  }
  boolean inLoop = false;
  while ((m1Speed != _lastM1Speed) && (m2Speed != _lastM2Speed))
  {
    if (inLoop)
    {
      delay(1);
    }
    inLoop = true;
    if (m1Speed > _lastM1Speed)
    {
      if ((m1Speed - _lastM1Speed) > speedStep)
      {
        _lastM1Speed += speedStep;
      }
      else
      {
        _lastM1Speed = m1Speed;
      }
    }
    else
    {
      if ((_lastM1Speed - m1Speed) > speedStep)
      {
        _lastM1Speed -= speedStep;
      }
      else
      {
        _lastM1Speed = m1Speed;
      }
    }
    if (m2Speed > _lastM2Speed)
    {
      if ((m2Speed - _lastM2Speed) > speedStep)
      {
        _lastM2Speed += speedStep;
      }
      else
      {
        _lastM2Speed = m2Speed;
      }
    }
    else
    {
      if ((_lastM2Speed - m2Speed) > speedStep)
      {
        _lastM2Speed -= speedStep;
      }
      else
      {
        _lastM2Speed = m2Speed;
      }
    }
    setM1Speed(_lastM1Speed);
    setM2Speed(_lastM2Speed);
  }
}

void Car::setM1Speed(int speed)
{
  _lastM1Speed = speed;
  boolean reverse = false;
  if (speed < 0)
  {
    speed = -speed; // make speed a positive quantity
    reverse = true; // preserve the direction
  }
  if (speed > 0xFF) // 0xFF = 255
    speed = 0xFF;
  if (speed < 10)
  {
    digitalWrite(pin_m2_a, LOW);
    digitalWrite(pin_m2_b, LOW);
  }
  else if (reverse)
  {
    digitalWrite(pin_m1_a, LOW);
    digitalWrite(pin_m1_b, HIGH);
  }
  else
  {
    digitalWrite(pin_m1_a, HIGH);
    digitalWrite(pin_m1_b, LOW);
  }
  analogWrite(pwm_m1, speed);
}

void Car::setM2Speed(int speed)
{
  _lastM2Speed = speed;
  boolean reverse = false;
  if (speed < 0)
  {
    speed = -speed; // make speed a positive quantity
    reverse = true; // preserve the direction
  }
  if (speed > 0xFF) // 0xFF = 255
    speed = 0xFF;
  if (speed < 10)
  {
    digitalWrite(pin_m2_a, LOW);
    digitalWrite(pin_m2_b, LOW);
  }
  else if (reverse)
  {
    digitalWrite(pin_m2_a, LOW);
    digitalWrite(pin_m2_b, HIGH);
  }
  else
  {
    digitalWrite(pin_m2_a, HIGH);
    digitalWrite(pin_m2_b, LOW);
  }
  analogWrite(pwm_m2, speed);
}

void Car::setMotorPower(boolean state)
{
  if (state)
  {
    digitalWrite(standby, HIGH);
  }
  else
  {
    digitalWrite(standby, LOW);
  }
}

int Car::getSpeed()
{
  int speed = _lastM1Speed + _lastM2Speed;
  if (speed < 0)
  {
    speed *= -1;
  }
  return speed/2;
}

void Car::stop()
{
  int speed = getSpeed();
  if(speed > 50) speed = 50;
  for (int i = speed; i >= 10; i -= 10)
  {
    setSpeeds(i, i);
    delay(15);
  }
  setSpeeds(0,0);
}