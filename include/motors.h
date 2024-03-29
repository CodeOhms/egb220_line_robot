#ifndef   MOTORS_H
#define   MOTORS_H

#include <avr/io.h>

// #include "pins.h"
#include "peripherals.h"

#define HBRIDGE_PINS_PER_M 2
#define PHASE 0
#define ENABLE 1

enum motor_direction
{
    forward,
    reverse,
    stop
};

/*
Provide a two dimensional array, for each motor, storing arrays of uint8_t numbers.
The 0th dimension represents a motor, and contains the array in the 1st dimension.
The 1st dimesnion represents the pins connected to an h bridge for, each motor.
I.e. the 1st dimension contains the pin numbers for the phase/direction and enable pins of each.
Each array will be copied.

Returns 0 if fast_pwm not already initialised!

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

void motor_close();

#ifdef ENV_ARDUINO
/*
Uses analogWrite() from Arduino, which writes out a square wave,
for pwm. This function uses a unit8_t (0 to 255) to specify
a duty cycle.
*/
void motor_move_direct(float speed, enum motor_direction direction, uint8_t enable_pin, uint8_t phase_pin);
#endif //ENV_ARDUINO

#ifdef ENV_AVR
typedef struct motors_info_t
{
    volatile uint8_t* port_regs[MOTORS_NUM];
    volatile uint8_t* direction_regs[MOTORS_NUM][HBRIDGE_PINS_PER_M];
    volatile uint8_t* compare_regs[MOTORS_NUM];
    const uint8_t pins_offset[MOTORS_NUM][HBRIDGE_PINS_PER_M]; // E.G. offset 6 with port E is PE6.
} motors_info;

extern const motors_info _motors;

// uint8_t motor_init(motors_info motors);

#endif //ENV_AVR
void motor_init();

void motor_move(float speed, enum motor_direction direction, uint8_t motor_selected);
#endif //MOTORS_H