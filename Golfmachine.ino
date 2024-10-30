#include "gm.h"


// pins
#define dirPin 2
#define stepPin 3
#define knopPin1 5
#define knopPin2 6
#define knopPin3 7

const long stepsPerRevolution = 7200; // aantal stappen per rondje, liefst deelbaar door 360 voor geen afrondingsproblemen
// standaard instellingen voor modi
const int draaiHoek1 = 30; // hoek in graden; 0 < draaiHoek <= 90; modus 1
const int draaiHoek2 = 45; // hoek in graden; 0 < draaiHoek <= 90; modus 2
const int draaiHoek3 = 90; // hoek in graden; 0 < draaiHoek <= 90; modus 3

// Snelheid
const long maxDel = 2000; // maximum vertraging na elke stap in microseconden; maxDel > minDel >= 500
const long minDel = 500; // minimum vertraging na elke stap in microseconden; maxDel > minDel >= 500
const int prec = 100; // bij het draaien van de draaiknop met x-aantal graden neemt de vertraging af/toe met prec aantal microseconden

// variabelen
int modus = 0;
int nieuweModus = 0; // nieuweModus is gelijk aan de startmodus; 0 = modus1, 1 = modus2, 2 = modus3 
int knopStatus[3]; 
int knopPin[3] = {knopPin1, knopPin2, knopPin3};
int draaiHoek[3] = {draaiHoek1, draaiHoek2, draaiHoek3};
gm gm;
void setup() {
  Serial.begin(9600);
  gm.initPins(dirPin, stepPin, knopPin1, knopPin2, knopPin3);
  
  for (int i = 0; i < 3; i++) {
    knopStatus[i] = digitalRead(knopPin[i]);
    if (knopStatus[i] == LOW) {
      nieuweModus = i;
    }
  }
}

void loop() {
  draaiMotor(true); // draai met de klok mee
  draaiMotor(false); // draai tegen de klok in
  draaiMotor(false); // draai tegen de klok in
  draaiMotor(true); // draai met de klok mee
}

void draaiMotor(bool cw) {
  modus = nieuweModus;
  if (draaiHoek[modus] > 90) {return;}
  for (int i = 0; i <(stepsPerRevolution/360)*draaiHoek[modus]; i++) {
    long snelheid = gm.calcSpeed(A0, maxDel, minDel, prec);
    gm.stepmotor(stepPin, dirPin, snelheid, cw);
    for (int j = 0; j < 3; j++) {
    knopStatus[j] = digitalRead(knopPin[j]);
    if (knopStatus[j] == LOW) {
      nieuweModus = j;
      Serial.print("nieuwe modus = "); Serial.println(nieuweModus);
    }
  }
  }

}
