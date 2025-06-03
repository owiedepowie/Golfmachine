/*
 * gm.h - Modified Stepper library for Wiring/Arduino - Version 1.1.0
 */

// ensure this library description is only included once
#ifndef GM_H
#define GM_H

// library interface description
class Stepper {
  public:
    Stepper(int number_of_steps, int motor_pin_1, int motor_pin_2,
                                 int motor_pin_3, int motor_pin_4,
                                 int button_pin_1, int button_pin_2,
                                 int button_pin_3, int potentiometer_pin,
                                 int minimum_velocity, int maximum_velocity,
                                 int bound);

    // speed setter method:
    void setSpeed();

    // mover method:
    void step(int number_of_steps);

    int version(void);

    int getMode(void);

    int getStep(void);

    int getCounter(void);

    // custom movement:

    void sinewave(int amplitude, int equilibrium, int period)

  private:
    void stepMotor(int this_step);

    int direction;            // Direction of rotation
    unsigned long step_delay; // delay between steps, in ms, based on speed
    int number_of_steps;      // total number of steps this motor can take
    int step_number;          // which step the motor is on


    // motor pin numbers:
    int motor_pin_1;
    int motor_pin_2;
    int motor_pin_3;
    int motor_pin_4;
    // input pins:
    int button_pin_1;
    int button_pin_2;
    int button_pin_3;
    int potentiometer_pin;
    // numbers:
    int minimum_velocity;
    int maximum_velocity;
    int bound;
    int mode;
    int counter;
    // button state:
    int lastButtonState;
    int currentButtonState;
    // sinewave function
    int amplitude;
    int equilibrium; // evenwichtsstand
    int period;
    bool sine;

    unsigned long last_step_time; // time stamp in us of when the last step was taken
};

#endif

