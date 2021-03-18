#if ENV_AVR == 1

#include <avr/io.h>

#include "motors.h"

// 0th index is for phase and 1st index is for enable pin on H bridge!
// uint8_t* _motor_l_pins_port_reg[MOTORS_NUM];
// uint8_t* _motor_l_pins_direction_reg[MOTORS_NUM];
// uint8_t* _motor_r_pins_port_reg[MOTORS_NUM];
// uint8_t* _motor_r_pins_direction_reg[MOTORS_NUM];
// uint8_t _motor_l_pins_offset[MOTORS_NUM];
// uint8_t _motor_r_pins_offset[MOTORS_NUM];

struct _motor_info _motor_l;
struct _motor_info _motor_r;

void motor_init(enum pins_mcu* motor_l_pins, enum pins_mcu* motor_r_pins)
{
    // Resolve registers.
        // Port registers:
    for(uint8_t i = 0; i < MOTORS_NUM; ++i)
    {
        _motor_l.port_regs[i] = resolve_port_reg(motor_l_pins[i]);
        _motor_r.port_regs[i] = resolve_port_reg(motor_r_pins[i]);
    }
        // Direction registers:
    for(uint8_t i = 0; i < MOTORS_NUM; ++i)
    {
        _motor_l.direction_regs[i] = resolve_direction_reg(motor_l_pins[i]);
        _motor_r.direction_regs[i] = resolve_direction_reg(motor_r_pins[i]);
    }

    // Setup the pin numbering offsets.
    for(uint8_t i = 0; i < MOTORS_NUM; ++i)
    {
        _motor_l.pins_offset[i] = resolve_pin_offset(motor_l_pins[i]);
        _motor_r.pins_offset[i] = resolve_pin_offset(motor_r_pins[i]);
    }

    // Setup control pins as outputs.
    for(uint8_t i = 0; HBRIDGE_PINS_PER_M; ++i)
    {
        uint8_t offset = _motor_l.pins_offset[i];
        *(_motor_l.direction_regs[i]) |= (1<<offset);
    }
    for(uint8_t i = 0; HBRIDGE_PINS_PER_M; ++i)
    {
        uint8_t offset = _motor_r.pins_offset[i];
        *(_motor_r.direction_regs[i]) |= (1<<offset);
    }
}

void motor_close()
{

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
            //TODO: deal with this going wrong HIGH PRIORITY.
            *(port_regs[PHASE]) &= ~(1<<pins_offset[PHASE]);
            speed_conv = 0;
            break;
    }

    // Activate motor.
        // Set direction.
        // TODO: finish implementation of motor running code!
    
    // digitalWrite(phase_pin, rot_dir);
        // Setup pwm signal.
    // analogWrite(enable_pin, speed_conv);
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