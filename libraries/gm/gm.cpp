#include "gm.h"



gm::gm() {
  
}

long gm::calcSpeed(byte potPin, long maxDel, long minDel, long prec, bool esp32) {
  int analogMax = 1023;
  if (esp32) {analogMax = 4095;}
  if (maxDel<=minDel) {return Serial.println("Error: Maximum delay must be greater than minimum delay");}
  if (minDel<500) {Serial.println("Error: Minimum delay must be greater or equal to 500");} 
  int analogValue = analogRead(potPin);
  int calcMaxDel = maxDel/prec;
  int calcMinDel = minDel/prec;
  return prec*map(analogValue, 0, analogMax, calcMaxDel, calcMinDel);
}

void gm::stepmotor(byte stepPin, byte dirPin, long v, bool cw) {
  
  if (cw) {
    digitalWrite(dirPin, HIGH);
  }
  else {
    digitalWrite(dirPin, LOW);
  }
  digitalWrite(stepPin, LOW);
  digitalWrite(stepPin, HIGH);
    delayMicroseconds(v);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(v);
  

}

void gm::initPins(byte dirPin, byte stepPin, byte buttonPin1, byte buttonPin2, byte buttonPin3) {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
}
