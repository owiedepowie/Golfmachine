#ifndef GM_H
#define GM_H

#include <Arduino.h>

class gm {
  
  private:
    byte stepPin;
    byte dirPin;
    byte buttonPin1;
    byte buttonPin2;
    byte buttonPin3;
    byte potPin;

  public:
  gm();

  long calcSpeed(byte potPin, long maxDel, long minDel, long prec, bool esp32);

  void stepmotor(byte stepPin, byte dirPin, long v, bool cw);
  
  void initPins(byte dirPin, byte stepPin, byte buttonPin1, byte buttonPin2, byte buttonPin3);
};

#endif
