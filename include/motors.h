#ifndef   MOTORS_H
#define   MOTORS_H

#include <Arduino.h>

#include "include/peripherals.h"


enum motor_direction
{
    forward,
    reverse,
    stop
};

/*
Provide a uint8_t array, for both motors,
filled with the pin numbers for
the input and enable pins of each.
Each array will be copied.

The pin numbers should be what is defined by
JDLuck's Arduino port.

For the pin arrays:
0th index is for phase and 1st is for enable pin on H bridge.
The phase pin sets direction of the motor with either 1 or 0.
The enable pin is used for pwm.
On the JDLuck Spark Mark II, mode pin is tied to VCC
meaning the H bridge operates in PHASE/ENABLE mode.
H Bridge datasheet: https://www.ti.com/product/DRV8835?HQS=TI-null-null-octopart-df-pf-null-wwe
Additional explaination of H bridge: https://www.pololu.com/product/2753
*/
void motor_init(uint8_t* motor_l_pins, uint8_t* motor_r_pins);

void motor_close();

/*
Uses analogWrite() from Arduino, which writes out a square wave,
for pwm. This function uses a unit8_t (0 to 255) to specify
a duty cycle.
*/
void motor_move_direct(float speed, enum motor_direction direction, int enable_pin, int phase_pin);


#endif //MOTORS_H