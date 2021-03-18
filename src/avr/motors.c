#ifdef ENV_AVR

#include <avr/io.h>
#include <avr/interrupt.h>

#include "motors.h"

struct _motor_info _motor_l;
struct _motor_info _motor_r;

void motor_init(enum pins_mcu* motor_l_pins, enum pins_mcu* motor_r_pins)
{
    // Resolve registers.
        // Port registers:
    for(uint8_t i = 0; i < HBRIDGE_PINS_PER_M; ++i)
    {
        _motor_l.port_regs[i] = resolve_port_reg(motor_l_pins[i]);
        _motor_r.port_regs[i] = resolve_port_reg(motor_r_pins[i]);
    }
        // Direction registers:
    for(uint8_t i = 0; i < HBRIDGE_PINS_PER_M; ++i)
    {
        _motor_l.direction_regs[i] = resolve_direction_reg(motor_l_pins[i]);
        _motor_r.direction_regs[i] = resolve_direction_reg(motor_r_pins[i]);
    }

    // Set up the pin numbering offsets.
    for(uint8_t i = 0; i < HBRIDGE_PINS_PER_M; ++i)
    {
        _motor_l.pins_offset[i] = resolve_pin_offset(motor_l_pins[i]);
        _motor_r.pins_offset[i] = resolve_pin_offset(motor_r_pins[i]);
    }

    // Set up control pins as outputs.
    for(uint8_t i = 0; HBRIDGE_PINS_PER_M; ++i)
    {
        uint8_t offset = _motor_l.pins_offset[i];
        *(_motor_l.direction_regs[i]) |= (1<<offset);

        offset = _motor_r.pins_offset[i];
        *(_motor_r.direction_regs[i]) |= (1<<offset);
    }

    // Set up pwm functions for each motor.
        // FIXME: set up pwm functions for each motor.
    
}

void motor_close()
{
    // Turn off the motors.
        // Set both the phase and enable pins low.
    for(uint8_t i = 0; i < HBRIDGE_PINS_PER_M; ++i)
    {
        uint8_t offset = _motor_l.pins_offset[i];
        *(_motor_l.port_regs[i]) &= ~(1<<offset);

        offset = _motor_r.pins_offset[i];
        *(_motor_r.port_regs[i]) &= ~(1<<offset);
    }

    // Clear the data.
        // Port registers:
    for(uint8_t i = 0; i < HBRIDGE_PINS_PER_M; ++i)
    {
        _motor_l.port_regs[i] = 0;
        _motor_r.port_regs[i] = 0;
    }
        // Direction registers:
    for(uint8_t i = 0; i < HBRIDGE_PINS_PER_M; ++i)
    {
        _motor_l.direction_regs[i] = 0;
        _motor_r.direction_regs[i] = 0;
    }

        // Pin numbering offsets:
    for(uint8_t i = 0; i < HBRIDGE_PINS_PER_M; ++i)
    {
        _motor_l.pins_offset[i] = 0;
        _motor_r.pins_offset[i] = 0;
    }

    // Reset control pins.
    for(uint8_t i = 0; HBRIDGE_PINS_PER_M; ++i)
    {
        uint8_t offset = _motor_l.pins_offset[i];
        *(_motor_l.direction_regs[i]) &= ~(1<<offset);

        offset = _motor_r.pins_offset[i];
        *(_motor_r.direction_regs[i]) &= ~(1<<offset);
    }
}

uint8_t speed_to_pwm_value(float speed)
{
    if(!(speed >= 0.0 && speed <= 100.0))
    {
        return 0;
    }

    //100%:255 divide both by 100 --> 1%:2.55. Conv to int is needed.
    uint8_t speed_conv = speed * 2.55; // Will truncate.

    return speed_conv;
}

void motor_move_direct(float speed, enum motor_direction direction, struct _motor_info motor_pins)
{
    // Convert speed between 0 to 100% to between 0 to 255.
    uint8_t speed_conv = speed_to_pwm_value(speed);

    // Retrieve the variables needed from the motor_pins.
    uint8_t* port_regs[HBRIDGE_PINS_PER_M]  = motor_pins.port_regs;
    uint8_t pins_offset[HBRIDGE_PINS_PER_M] = motor_pins.pins_offset;

    // Activate motor.
    switch(direction)
    {
        case forward:
            // Set phase pin low.
            *(port_regs[PHASE]) &= ~(1<<pins_offset[PHASE]);
            break;
        
        case reverse:
            // Set phase pin high.
            *(port_regs[PHASE]) |= (1<<pins_offset[PHASE]);
            break;
        
        case stop:
            // Set phase pin low, and use a duty cycle of 0%.
            *(port_regs[PHASE]) &= ~(1<<pins_offset[PHASE]);
            speed_conv = 0;
            break;

        default:
            // Something went VERY WRONG.
            // TODO: deal with this going wrong HIGH PRIORITY.
            *(port_regs[PHASE]) &= ~(1<<pins_offset[PHASE]);
            speed_conv = 0;
            break;
    }

    // Setup pwm signal.
    // analogWrite(enable_pin, speed_conv);
    //FIXME: set up pwm signal for given motor.
}

void motor_move(float speed, enum motor_direction direction, enum motors motor)
{
    struct _motor_info motor_selected;
    switch(motor)
    {
        case motor_left:
            motor_selected = _motor_l;
            break;
        
        case motor_right:
            motor_selected = _motor_r;
            break;
    }

    motor_move_direct(speed, direction, motor_selected);
}

#endif //ENV_AVR