#include <Arduino.h>
#include <esp32-hal-uart.h>
#include "Golfmachine.h"
// pins
#define potPin 1
#define knopPin1 40
#define knopPin2 41
#define knopPin3 42

typedef enum
{
  ST_IDLE,
  ST_RUNNING,
  ST_UNKNOWN,
} STATES;

const unsigned long runningTimeMs = 30000;

const long stepsPerRevolution = 360; // aantal stappen per rotatie
// standaard instellingen voor de modi
const int draaiHoek1 = 30; // hoek in graden; modus 1
const int draaiHoek2 = 45; // hoek in graden; modus 2
const int draaiHoek3 = 90; // hoek in graden; modus 3

// Snelheid
const int minimumSnelheid = 20; // in rotaties per minuut
const int maximumSnelheid = 100; // in rotaties per minuut
const int grenswaarde = 30; // als de snelheid kleiner of gelijk is aan deze waarde stopt de beweging; in rotaties per minuut

STATES state;
unsigned long startTime;
HardwareSerial serial(0);

Stepper gm = Stepper(stepsPerRevolution, 7, 6, 5, 4, knopPin1, knopPin2, knopPin3, potPin, minimumSnelheid, maximumSnelheid, grenswaarde);

void setup() 
{
  state = ST_IDLE;
  serial.begin(115200/*, SERIAL_8N1, -1, -1*/);
  serial.println("Start-up\r\n");
  
  //gm.setOutputPercentage(70);
}

void loop() 
{
  static STATES tmpState = ST_UNKNOWN;
  static int tmpCounter = 10;
  int counter = gm.getCounter(); // leest hoe vaak knop 3 is ingedrukt
  int modus = gm.getMode(); // leest welke knop als laatst is ingedrukt
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
  /*
  switch(modus) {
    case 0: 
    { // als knop 1 als laatst is ingedrukt ...
      if (stapGetal != 0) 
      { // beweegt de plaat naar de beginpositie
        if (stapGetal > stepsPerRevolution/2) 
        {
          gm.step(stepsPerRevolution - stapGetal);
        }
        if (stapGetal <= stepsPerRevolution/2) 
        {
          gm.step(-stapGetal);
        }
      }
      gm.step((stepsPerRevolution/360)*draaiHoek1); // draai met draaihoek1 graden 1 keer rechtsom, 
      gm.step(-2*(stepsPerRevolution/360)*draaiHoek1); // 2 keer linksom,
      gm.step((stepsPerRevolution/360)*draaiHoek1); // en 1 keer rechtsom
      break; 
    }
    case 1: 
    { // als knop 2 als laatst is ingedrukt ...
      if (stapGetal != 0) 
      { // beweegt de plaat naar de beginpositie
        if (stapGetal > stepsPerRevolution/2) 
        {
          gm.step(stepsPerRevolution - stapGetal);
        }
        if (stapGetal <= stepsPerRevolution/2) 
        {
          gm.step(-stapGetal);
        }
      }
      gm.step((stepsPerRevolution/360)*draaiHoek2); // draai met draaihoek2 graden 1 keer rechtsom, 
      gm.step(-2*(stepsPerRevolution/360)*draaiHoek2); // 2 keer linksom,
      gm.step((stepsPerRevolution/360)*draaiHoek2); // en 1 keer rechtsom
      break; 
    }
    case 2: 
    { // als knop 3 als laatst is ingedrukt ...
      switch(counter) 
      { 
        case 0: 
        {
          if (stapGetal != 0) 
          { // beweegt de plaat naar de beginpositie
            if (stapGetal > stepsPerRevolution/2) 
            {
              gm.step(stepsPerRevolution - stapGetal);
            }
            if (stapGetal <= stepsPerRevolution/2) 
            {
              gm.step(-stapGetal);
            }
          }

          gm.step((stepsPerRevolution/360)*draaiHoek1); // draai met draaihoek1 graden 1 keer rechtsom, 
          gm.step(-2*(stepsPerRevolution/360)*draaiHoek1); // 2 keer linksom,
          gm.step((stepsPerRevolution/360)*draaiHoek1); // en 1 keer rechtsom
          break;
        }

        case 1: 
        {
          if (stapGetal != 0) 
          { // beweegt de plaat naar de beginpositie
            if (stapGetal > stepsPerRevolution/2) 
            {
              gm.step(stepsPerRevolution - stapGetal);
            }

            if (stapGetal <= stepsPerRevolution/2) 
            {
              gm.step(-stapGetal);
            }
          }

          gm.step((stepsPerRevolution/360)*draaiHoek2); // draai met draaihoek2 graden 1 keer rechtsom, 
          gm.step(-2*(stepsPerRevolution/360)*draaiHoek2); // 2 keer linksom,
          gm.step((stepsPerRevolution/360)*draaiHoek2); // en 1 keer rechtsom
          break;
        }
        case 2: 
        {
          if (stapGetal != 0)
          { // beweegt de plaat naar de beginpositie
            if (stapGetal > stepsPerRevolution/2) 
            {
              gm.step(stepsPerRevolution - stapGetal);
            }
            
            if (stapGetal <= stepsPerRevolution/2) 
            {
              gm.step(-stapGetal);
            }
          }

          gm.step((stepsPerRevolution/360)*draaiHoek3); // draai met draaihoek3 graden 1 keer rechtsom, 
          gm.step(-2*(stepsPerRevolution/360)*draaiHoek3); // 2 keer linksom,
          gm.step((stepsPerRevolution/360)*draaiHoek3); // en 1 keer rechtsom
          break;
        }

        case 3: 
        {
          gm.step(1);
          break;
        }
      }
      break; 
    }
  }
  */
}
