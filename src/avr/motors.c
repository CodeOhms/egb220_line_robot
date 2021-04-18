#ifdef ENV_AVR

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "motors.h"
#include "fast_pwm.h"

motors_info _motors;

uint8_t motor_init(motors_info motors)
{
    if(!_fast_pwm_is_initialised)
    {
        return 0;
    }

    for(uint8_t m = 0; m < MOTORS_NUM; ++m)
    {
        // Copy data to local struct:
            // Copy compare registers:
        _motors.compare_regs[m] = motors.compare_regs[m];
            // Copy direction registers:
        _motors.direction_regs[m] = motors.direction_regs[m];

        for(uint8_t p = 0; p < HBRIDGE_PINS_PER_M; ++p)
        {
                // Copy port registers:
            _motors.port_regs[m][p] = motors.port_regs[m][p];
                // Copy pin offsets:
            _motors.pins_offset[m][p] = motors.pins_offset[m][p];

            // Setup pins as outputs:
            _motors.direction_regs[m][p] = ;
        }
    }




    // for(uint8_t motor = 0; motor < MOTORS_NUM; ++motor)
    // {
    //     // Set up enable pins alises:
    //     _motors.enable_pin[motor] = motors_pins[motor][ENABLE];

    // // Resolve registers:
    //     // Port registers:
    //     _motors.port_regs[motor][PHASE] = resolve_port_reg(motors_pins[motor][PHASE]);
    //     _motors.port_regs[motor][ENABLE] = resolve_port_reg(motors_pins[motor][ENABLE]);

    //     // Direction registers:
    //     _motors.direction_regs[motor][PHASE] = resolve_direction_reg(motors_pins[motor][PHASE]);
    //     _motors.direction_regs[motor][ENABLE] = resolve_direction_reg(motors_pins[motor][ENABLE]);
    
    // // Resolve pin numbering offsets:
    //     _motors.pins_offset[motor][PHASE] = resolve_pin_offset(motors_pins[motor][PHASE]);
    //     _motors.pins_offset[motor][ENABLE] = resolve_pin_offset(motors_pins[motor][ENABLE]);

    // // Setup pins as outputs:
    //     uint8_t offset = _motors.pins_offset[motor][PHASE];
    //     *(_motors.direction_regs[motor][PHASE]) |= (1<<offset); 
    //     offset = _motors.pins_offset[motor][ENABLE];
    //     *(_motors.direction_regs[motor][ENABLE]) |= (1<<offset);

    // // Set the compare match flags to zero to start.
    //     fast_pwm_set_compare_counter(motors_pins[motor][ENABLE], 0);
    // }

    return 1;
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

void motor_move_direct(float speed, enum motor_direction direction, uint8_t motor_selected)
{
    // Convert speed between 0 to 100% to between 0 to 255.
    uint8_t speed_conv = speed_to_pwm_value(speed);

    // Retrieve the variables needed from `motor_selected`.
        // This is a pointer to the first element in the array, saves copying it :).
    volatile uint8_t* phase_pin_port_reg = _motors.port_regs[motor_selected][PHASE];
    uint8_t* pins_offset = _motors.pins_offset[motor_selected];

    // Activate motor.
    switch(direction)
    {
        case forward:
            // Set phase pin low.
            *(phase_pin_port_reg) &= ~(1<<pins_offset[PHASE]);
            break;
        
        case reverse:
            // Set phase pin high.
            *(phase_pin_port_reg) |= (1<<pins_offset[PHASE]);
            break;
        
        case stop:
            // Set phase pin low, and use a duty cycle of 0%.
            *(phase_pin_port_reg) &= ~(1<<pins_offset[PHASE]);
            speed_conv = 0;
            break;

        default:
            // Something went VERY WRONG.
            // TODO: deal with this going wrong HIGH PRIORITY.
            *(phase_pin_port_reg) &= ~(1<<pins_offset[PHASE]);
            speed_conv = 0;
            break;
    }

    // Set the compare threshold for the PWM to set speed.
    fast_pwm_set_compare_counter(_motors.enable_pin[motor_selected], speed_conv);
}

void motor_move(float speed, enum motor_direction direction, enum motors motor)
{
    motor_move_direct(speed, direction, (uint8_t) motor);
}

#endif //ENV_AVR