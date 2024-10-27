#ifndef GM_H
#define GM_H

#include <Arduino.h>

class gm {
  
  private:
    byte stepPin;
    byte dirPin;
    byte buttonPin;
    byte potPin;

  public:
  gm();
  bool readButton(byte buttonPin, int currentButtonState);

  void calcSpeed(byte potPin, long maxDel, long minDel, long prec);

  void stepmotor(byte stepPin, byte dirPin, long SPR, long v, int a, bool cw);
  
};

#endif