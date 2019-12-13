#ifndef SRC_CAR_H_
#define SRC_CAR_H_
#include <Arduino.h>

class Car
{
  private:
    int pin_m1_a, pin_m1_b, pin_m2_a, pin_m2_b, pwm_m1, pwm_m2, standby;
    boolean isVirtual;
    int _lastM1Speed =0;
    int _lastM2Speed =0;

  public:
    Car(int pin_m1_a, int pin_m1_b,int pwm_m1,int  pin_m2_a, int pin_m2_b,int pwm2,int standby,boolean isVirtual);
    void setSpeeds(int m1Speed, int m2Speed);
    void setSpeeds(int m1Speed, int m2Speed,int speedStep);
    void setM1Speed(int speed);
    void setM2Speed(int speed);
    void setMotorPower(boolean state);
    int getSpeed();
    void stop();
};
#endif /* SRC_AVRCLOCK_H_ */