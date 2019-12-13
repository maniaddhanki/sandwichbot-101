#ifndef SRC_BUTTONS_H_
#define SRC_BUTTONS_H_
#include <Arduino.h>
class Buttons
{
  private:
   unsigned static int _btPins[3];
  public:
    void static init(int pin1,int pin2,int pin3);
    boolean static getState(int buttonNum);
    int static getNextButtonPressed();
};
#endif /* SRC_BUTTONS_H_ */