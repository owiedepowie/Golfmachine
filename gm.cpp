/*
 gm.cpp - Modified Stepper library for Wiring/Arduino - Version 1.1.0
*/
#include "Arduino.h"
#include "gm.h"

/*
 *   constructor for four-pin version
 *   Sets which wires should control the motor.
 */
Stepper::Stepper(int number_of_steps, int motor_pin_1, int motor_pin_2,
                                      int motor_pin_3, int motor_pin_4,
                                      int button_pin_1, int button_pin_2,
                                      int button_pin_3, int potentiometer_pin,
                                      int minimum_velocity, int maximum_velocity,
                                      int bound)
{
  this->step_number = 0;    // which step the motor is on
  this->direction = 0;      // motor direction
  this->last_step_time = 0; // time stamp in us of the last step taken
  this->number_of_steps = number_of_steps; // total number of steps for this motor

  // Arduino pins for the motor control connection:
  this->motor_pin_1 = motor_pin_1;
  this->motor_pin_2 = motor_pin_2;
  this->motor_pin_3 = motor_pin_3;
  this->motor_pin_4 = motor_pin_4;
  // input pins:
  this->button_pin_1 = button_pin_1;
  this->button_pin_2 = button_pin_2;
  this->button_pin_3 = button_pin_3;
  this->potentiometer_pin = potentiometer_pin;
  // numbers:
  this->mode = 0;
  this->counter = 0;
  this->minimum_velocity = minimum_velocity;
  this->maximum_velocity = maximum_velocity;
  this->bound = bound;
  // button state:
  this->currentButtonState = 0;
  this->lastButtonState = 0;
  // sinewave function
  this->amplitude = 0;
  this->equilibrium = 0;
  this->period = 0;
  this->sine = false;

  // setup the pins on the microcontroller:

  pinMode(this->motor_pin_1, OUTPUT);
  pinMode(this->motor_pin_2, OUTPUT);
  pinMode(this->motor_pin_3, OUTPUT);
  pinMode(this->motor_pin_4, OUTPUT);

  pinMode(this->button_pin_1, INPUT_PULLUP);
  pinMode(this->button_pin_2, INPUT_PULLUP);
  pinMode(this->button_pin_3, INPUT_PULLUP);
}


/*
 * Sets the speed in revs per minute
 */
void Stepper::setSpeed()
{
  this->step_delay = 60L * 1000L * 1000L / this->number_of_steps / speed;
}

/*
 * Moves the motor steps_to_move steps.  If the number is negative,
 * the motor moves in the reverse direction.
 */
void Stepper::step(int steps_to_move)
{
  int steps_left = abs(steps_to_move);  // how many steps to take

  // determine direction based on whether steps_to_mode is + or -:
  if (steps_to_move > 0) { this->direction = 1; }
  if (steps_to_move < 0) { this->direction = 0; }
  


  // decrement the number of steps, moving one step each time:
  while (steps_left > 0)
  {
      if(!sine) {
        int analogValue = analogRead(this->potentiometer_pin);
        int speed = map(analogValue, 0, 4095, maximum_velocity, minimum_velocity);
        if (speed <= bound) {
          return;
        }
      }
      setSpeed();
    unsigned long now = micros();
    // move only if the appropriate delay has passed:
    if (now - this->last_step_time >= this->step_delay)
    {
      // get the timeStamp of when you stepped:
      this->last_step_time = now;
      // increment or decrement the step number,
      // depending on direction:
      if (this->direction == 1)
      {
        this->step_number++;
        if (this->step_number == this->number_of_steps) {
          this->step_number = 0;
        }
      }
      else
      {
        if (this->step_number == 0) {
          this->step_number = this->number_of_steps;
        }
        this->step_number--;
      }
      // decrement the steps left:
      steps_left--;
      // step the motor to step number 0, 1, ..., {3 or 10}
      
      
      stepMotor(this->step_number % 4);
      this->lastButtonState = this->currentButtonState;
      this->currentButtonState = digitalRead(button_pin_3);
      if (digitalRead(button_pin_1) == LOW) {
      this->mode = 0;
      this->counter = 0;
      }
      if (digitalRead(button_pin_2) == LOW) {
      this->mode = 1;
      this->counter = 0;
      }
      if (this->lastButtonState == HIGH && this->currentButtonState == LOW) {
      this->mode = 2;
      this->counter++;
      if (this->counter > 3) {
        this->counter = 0;
      }
      }
    }
  }
}

/*
 * Moves the motor forward or backwards.
 */
void Stepper::stepMotor(int thisStep)
{
    switch (thisStep) {
      case 0:  // 1010
        digitalWrite(motor_pin_1, HIGH);
        digitalWrite(motor_pin_2, LOW);
        digitalWrite(motor_pin_3, HIGH);
        digitalWrite(motor_pin_4, LOW);
      break;
      case 1:  // 0110
        digitalWrite(motor_pin_1, LOW);
        digitalWrite(motor_pin_2, HIGH);
        digitalWrite(motor_pin_3, HIGH);
        digitalWrite(motor_pin_4, LOW);
      break;
      case 2:  //0101
        digitalWrite(motor_pin_1, LOW);
        digitalWrite(motor_pin_2, HIGH);
        digitalWrite(motor_pin_3, LOW);
        digitalWrite(motor_pin_4, HIGH);
      break;
      case 3:  //1001
        digitalWrite(motor_pin_1, HIGH);
        digitalWrite(motor_pin_2, LOW);
        digitalWrite(motor_pin_3, LOW);
        digitalWrite(motor_pin_4, HIGH);
      break;
    }
}

void Stepper::sinewave(int amplitude, int equilibrium, int period) // moves the plate while controlling the speed in a sinewave for one period
{
  this->sine = true;
  int currentMillis = millis();
  while(millis() - currentMillis < this->period*1000) {
  int speed = this->equilibrium + this->amplitude*sin((2*M_PI/this->period)*(millis()/1000));
  step(1);
  }
}
int Stepper::getMode(void) 
{
  return mode;
}
int Stepper::getStep(void) 
{
  return step_number;
}
int Stepper::getCounter(void) 
{
  return counter;
}