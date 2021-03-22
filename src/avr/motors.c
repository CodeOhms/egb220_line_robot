#ifdef ENV_AVR

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "motors.h"
#include "fast_pwm.h"

// struct _motor_info _motor_l;
// struct _motor_info _motor_r;
struct _motor_info _motors;

/*
Callback functions that is given to the fast pwm ISR.
Responsible for toggling the enable pin, for
specified motor, to generate a PWM signal.
*/
// void motor_l_pwm_event(void)
// {
//     uint8_t offset = _motor_l.pins_offset[ENABLE];
//     *(_motor_l.direction_regs[ENABLE]) ^= (1<<offset);
// }
// void motor_r_pwm_event(void)
// {
//     uint8_t offset = _motor_r.pins_offset[ENABLE];
//     *(_motor_r.direction_regs[ENABLE]) ^= (1<<offset);
// }

/*
This is only needed during initialisation and shutdown of this code.
*/
void get_motor_port_regs(volatile uint8_t** port_registers)
{

}

// uint8_t motor_init(enum pins_mcu* motor_l_pins, enum pins_mcu* motor_r_pins)
uint8_t motor_init(enum pins_mcu motors_pins[MOTORS_NUM][HBRIDGE_PINS_PER_M])
{
    // Set up pwm device identifiers.
    // _motor_l.motor_selected = pwm_motor_left;
    // _motor_r.motor_selected = pwm_motor_right;

    if(!_fast_pwm_is_initialised)
    {
        return 0;
    }

    for(uint8_t motor = 0; motor < MOTORS_NUM; ++motor)
    {
        // Set up enable pins alises:
        _motors.enable_pin[motor] = motors_pins[motor][ENABLE];

        // uint8_t index_phase = arrays_multidim_get_index(motor, HBRIDGE_PINS_PER_M, PHASE);
        // uint8_t index_enable = arrays_multidim_get_index(motor, HBRIDGE_PINS_PER_M, ENABLE);

    // Resolve registers:
        // Port registers:
        _motors.port_regs[motor][PHASE] = resolve_port_reg(motors_pins[motor][PHASE]);
        _motors.port_regs[motor][ENABLE] = resolve_port_reg(motors_pins[motor][ENABLE]);

        // Direction registers:
        _motors.direction_regs[motor][PHASE] = resolve_direction_reg(motors_pins[motor][PHASE]);
        _motors.direction_regs[motor][ENABLE] = resolve_direction_reg(motors_pins[motor][ENABLE]);
    
    // Resolve pin numbering offsets:
        _motors.pins_offset[motor][PHASE] = resolve_pin_offset(motors_pins[motor][PHASE]);
        _motors.pins_offset[motor][ENABLE] = resolve_pin_offset(motors_pins[motor][ENABLE]);

    // Setup pins as outputs:
        uint8_t offset = _motors.pins_offset[motor][PHASE];
        *(_motors.direction_regs[motor][PHASE]) |= (1<<offset); 
        offset = _motors.pins_offset[motor][ENABLE];
        *(_motors.direction_regs[motor][ENABLE]) |= (1<<offset);

    // Set the compare match flags to zero to start.
        fast_pwm_set_compare_counter(motors_pins[motor][ENABLE], 0);
    }

    return 1;

//     // Set enable pin:
//     _motor_l.enable_pin = motor_l_pins[ENABLE];
//     _motor_r.enable_pin = motor_r_pins[ENABLE];

//     // Resolve registers:
//         // Port registers (phase pin only):
//     _motor_l.phase_pin_port_reg = resolve_port_reg(motor_l_pins[PHASE]);
//     _motor_r.phase_pin_port_reg = resolve_port_reg(motor_r_pins[PHASE]);

//     for(uint8_t phase_enable_pin = 0; phase_enable_pin < HBRIDGE_PINS_PER_M; ++phase_enable_pin)
//     {
//         // Direction registers:
//         _motor_l.direction_regs[phase_enable_pin] = resolve_direction_reg(motor_l_pins[phase_enable_pin]);
//         _motor_r.direction_regs[phase_enable_pin] = resolve_direction_reg(motor_r_pins[phase_enable_pin]);

//     // Set up the pin numbering offsets.
//         _motor_l.pins_offset[phase_enable_pin] = resolve_pin_offset(motor_l_pins[phase_enable_pin]);
//         _motor_r.pins_offset[phase_enable_pin] = resolve_pin_offset(motor_r_pins[phase_enable_pin]);
//     }

//     // Set up control pins as outputs.
//         // Get the port registers for this:
//     volatile uint8_t*** port_registers = (uint8_t*) malloc(MOTORS_NUM * HBRIDGE_PINS_PER_M * sizeof(uint8_t*));
//     for(uint8_t motor = 0; motor < MOTORS_NUM; ++motor)
//     {
//         for(uint8_t pin = 0; pin < HBRIDGE_PINS_PER_M; ++pin)
//         {
//             uint8_t offset = 
//             *(port_registers[motor][pin]) |= (1<<offset);
//         }
//     }

//     uint8_t offset = _motor_l.pins_offset[PHASE];
//     *(_motor_l.phase_pin_port_reg) |= (1<<offset);

//     offset = _motor_r.pins_offset[phase_enable_pin];
//     *(_motor_r.direction_regs[phase_enable_pin]) |= (1<<offset);
//     //

//     // Set the compare match flags to zero to start.
//     fast_pwm_set_compare_counter(_motor_l.enable_pin, 0);
//     fast_pwm_set_compare_counter(_motor_r.enable_pin, 0);
// }

// void motor_close()
// {
//     // Turn off the motors.
//         // Set the phase pin logic levels low.
//     *(_motor_l.phase_pin_port_reg) &= ~(1<<_motor_l.pins_offset[PHASE]);
//     *(_motor_r.phase_pin_port_reg) &= ~(1<<_motor_r.pins_offset[PHASE]);
    
//         // Set the pwm compare match flags to zero.
//     fast_pwm_set_compare_counter(_motor_l.enable_pin, 0);
//     fast_pwm_set_compare_counter(_motor_r.enable_pin, 0);

//         // Set both the phase and enable pins' direction registers low.
//     for(uint8_t phase_enable_pin = 0; phase_enable_pin < HBRIDGE_PINS_PER_M; ++phase_enable_pin)
//     {
//         uint8_t offset = _motor_l.pins_offset[phase_enable_pin];
//         *(_motor_l.direction_regs[phase_enable_pin]) &= ~(1<<offset);

//         offset = _motor_r.pins_offset[phase_enable_pin];
//         *(_motor_r.direction_regs[phase_enable_pin]) &= ~(1<<offset);
//     }

//     // Clear the data.
//         // Port registers:
//     _motor_l.phase_pin_port_reg = 0;
//     _motor_r.phase_pin_port_reg = 0;
//     for(uint8_t i = 0; i < HBRIDGE_PINS_PER_M; ++i)
//     {
//         // Direction registers:
//         _motor_l.direction_regs[i] = 0;
//         _motor_r.direction_regs[i] = 0;

//         // Pin numbering offsets:
//         _motor_l.pins_offset[i] = 0;
//         _motor_r.pins_offset[i] = 0;
//     }

//         // Enable pin alises:
//     _motor_l.enable_pin = 0;
//     _motor_r.enable_pin = 0;
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
    // struct _motor_info* motor_selected;
    // switch(motor)
    // {
    //     case motor_left:
    //         motor_selected = &_motor_l;
    //         break;
        
    //     case motor_right:
    //         motor_selected = &_motor_r;
    //         break;
    // }

    motor_move_direct(speed, direction, (uint8_t) motor);
}

#endif //ENV_AVR