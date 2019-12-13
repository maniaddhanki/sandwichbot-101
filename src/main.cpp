#include "main.h"
#include "Car.h"
#include "RCSensor.h"
#include "LED.h"
#include "Buttons.h"
#include "SensorService.h"
#include "RCSensor.h"

int pin_m1_a[5] = {33, 35, 35, 35, 35};
int pin_m1_b[5] = {35, 33, 33, 33, 33};
int pin_m2_a[5] = {39, 39, 41, 41, 41};
int pin_m2_b[5] = {41, 41, 39, 39, 39};

int pwm_m1 = 2;
int pwm_m2 = 4;
int standby = 37;
int pin_bt1 = 42;
int pin_bt2 = 40;
int pin_bt3 = 38;
int pin_led1 = A4;
int pin_led2 = A2;
int pin_led3 = A0;
int pin_battery = A9;
uint16_t sensorValues[11] = {0};
Car car = Car(pin_m1_a[BOT_ID], pin_m1_b[BOT_ID], pwm_m1, pin_m2_a[BOT_ID], pin_m2_b[BOT_ID], pwm_m2, standby, false);
RCSensor rcsensors = RCSensor((const int[]){
                                  30, 28, 26, 24, 22, 21, 23, 25, 27, 29, 31},
                              false);
SensorService sensorService = SensorService();

void setup()
{
  // put your setup code here, to run once
  delay(100);
  LED::init(pin_led1, pin_led2, pin_led3);
  Buttons::init(pin_bt1, pin_bt2, pin_bt3);

  Serial.begin(115200);
  delay(100);
  Serial.println("Begins");
  pinMode(pin_battery, INPUT);
  sensorService.loadCalibratedLineSensors(&rcsensors);
  sensorService.loadCalibratedSideSensors(&rcsensors);
  Serial.println("Press 1 for Line follower");
  Serial.println("Press 2 for wall follower");
  car.setMotorPower(true);
  if (Buttons::getNextButtonPressed() == 2)
  {
    //code to follow wall
  }
  else
  {
    // code to follow line
  }
}

void loop()
{
  //loop task
}
