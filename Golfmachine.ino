#include <Arduino.h>
#include <esp32-hal-uart.h>
#include "Golfmachine.h"
// pins
#define potPin 1
#define knopPin 42

#define outputPerc  50

typedef enum
{
  ST_IDLE,
  ST_RUNNING,
  ST_UNKNOWN,
} STATES;

const unsigned long runningTimeMs = 30000;

const long stepsPerRevolution = 360; // aantal stappen per rotatie

// Snelheid
const int minimumSnelheid = 20; // in rotaties per minuut
const int maximumSnelheid = 100; // in rotaties per minuut
const int grenswaarde = 30; // als de snelheid kleiner of gelijk is aan deze waarde stopt de beweging; in rotaties per minuut

STATES state;
unsigned long startTime;
HardwareSerial serial(0);

Stepper gm = Stepper(stepsPerRevolution, 7, 6, 5, 4, knopPin, potPin, minimumSnelheid, maximumSnelheid, grenswaarde);

void setup() 
{
  state = ST_IDLE;
  serial.begin(115200/*, SERIAL_8N1, -1, -1*/);
  serial.println("Start-up\r\n");
  
  gm.setOutputPercentage(outputPerc);
}

void loop() 
{
  static STATES tmpState = ST_UNKNOWN;
  static int tmpCounter = 10;
  int counter = gm.getCounter(); // leest hoe vaak knop 3 is ingedrukt
  int stapGetal = gm.getStep(); // leest het aantal stappen dat de motor heeft gedaan sinds de beginpositie
  
  switch (state)
  {
    case ST_IDLE:
    {
      gm.step(0);   // to read button

      if (counter == 1)     // button pressed to start movement
      {
        startTime = millis();
        state = ST_RUNNING;
      }
    }
    break;

    case ST_RUNNING:
    {
      // time elapsed or button pressed again?
      if (((millis() - startTime) > runningTimeMs) || (counter == 0))
      {
        gm.step(0);     // set to idle
        gm.setCounter(0);
        state = ST_IDLE;
      }
      else
      {
        gm.step(1);
      }
    }
    break;
  }

  if (tmpState != state)
  {
    tmpState = state;
    serial.printf("%d", state);
    serial.println(" state\r\n");
  }

  if (tmpCounter != counter)
  {
    tmpCounter = counter;
    serial.printf("%d", counter);
    serial.println(" counter\r\n");
  }
}
