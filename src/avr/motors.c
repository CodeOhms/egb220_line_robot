#ifdef ENV_AVR

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "motors.h"
#include "fast_pwm.h"

void motor_init()
{
    for(uint8_t m = 0; m < MOTORS_NUM; ++m)
    {
    // Setup pins as outputs:
        for(uint8_t p = 0; p < HBRIDGE_PINS_PER_M; ++p)
        {
            *(_motors.direction_regs[m][p]) |= (1<<_motors.pins_offset[m][p]);
        }

    // Set compare match flags to zero:
        *(_motors.compare_regs[m]) = 0;
    }
}

uint8_t speed_to_pwm_value(float speed)
{
    if(!(speed >= 0.0 && speed <= 100.0))
    {
        return 0;
    }

    // 100%:255 divide both by 100 --> 1%:2.55. Conv to int is needed.
    uint8_t speed_conv = speed * 2.55; // Will truncate.

    return speed_conv;
}

void motor_move(float speed, enum motor_direction direction, uint8_t motor_selected)
{
    // Convert speed between 0 to 100% to between 0 to 255.
    uint8_t speed_conv = speed_to_pwm_value(speed);

    switch(direction)
    {
        case forward:
            // Set phase pin low.
            *(_motors.port_regs[motor_selected]) &= ~(1<<_motors.pins_offset[motor_selected][PHASE]);
            break;
        
        case reverse:
            // Set phase pin high.
            *(_motors.port_regs[motor_selected]) |= (1<<_motors.pins_offset[motor_selected][PHASE]);
            break;
        
        case stop:
            // Set phase pin low, and use a duty cycle of 0%.
            *(_motors.port_regs[motor_selected]) &= ~(1<<_motors.pins_offset[motor_selected][PHASE]);
            speed_conv = 0;
            break;

        default:
            // Something went VERY WRONG.
            // TODO: deal with this going wrong HIGH PRIORITY.
            *(_motors.port_regs[motor_selected]) &= ~(1<<_motors.pins_offset[motor_selected][PHASE]);
            speed_conv = 0;
            break;
    }

    // Set the compare threshold for the PWM to set speed.
    *(_motors.compare_regs[motor_selected]) = speed_conv;
}

#endif //ENV_AVR