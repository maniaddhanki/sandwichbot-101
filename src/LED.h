#ifndef SRC_LED_H_
#define SRC_LED_H_
#include <Arduino.h>
class LED
{
  private:
   unsigned static int _ledPins[3];
   unsigned static int _blinkTime[3];
   volatile boolean static _isBlinking[3];
   int static counter;
   void static setupTimmer5ForInterrupts();
   void static stopTimmer5Interrupts();
   boolean static isLedBlinking();
   void static updateTimerInterrupt();
  public:
    void static init(int pin1,int pin2,int pin3);
    void static setLedOn(int ledNum);
    void static setLedOff(int ledNum);
    void static setLedBlink(int ledNum,int interval);
    void static updateLedStates();
    void static setLedState(int state1,int state2,int state3);
};
#endif /* SRC_LED_H_ */