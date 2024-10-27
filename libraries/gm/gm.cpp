#include "gm.h"



gm::gm() {
  
}

bool gm::readButton(byte buttonPin, int currentButtonState) {
  int lastButtonState = currentButtonState;
  currentButtonState = digitalRead(buttonPin);
  if(lastButtonState == HIGH && currentButtonState == LOW) {
    return true;
  } else {return false;}
}


void gm::calcSpeed(byte potPin, long maxDel, long minDel, long prec) {
  if (maxDel<=minDel) {return;}
  if (minDel<500) {return;} 
  int analogValue = analogRead(potPin);
  int calcMaxDel = maxDel/prec;
  int calcMinDel = minDel/prec;
  return prec*map(analogValue, 0, 1023, calcMaxDel, calcMinDel);
}

void gm::stepmotor(byte stepPin, byte dirPin, long SPR, long v, int a, bool cw) {
  if (cw) {
    digitalWrite(dirPin, HIGH);
  }
  else {
    digitalWrite(dirPin, LOW);
  }
  for (int i = 0; i < (SPR/360)*a; i++) {
  digitalWrite(stepPin, HIGH);
    delayMicroseconds(v);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(v);
  }
}