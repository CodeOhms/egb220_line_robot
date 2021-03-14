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
        _motor_l.port_reg[i] = resolve_port_reg(motor_l_pins[i]);
        _motor_r.port_reg[i] = resolve_port_reg(motor_r_pins[i]);
    }
        // Direction registers:
    for(uint8_t i = 0; i < MOTORS_NUM; ++i)
    {
        _motor_l.direction_reg[i] = resolve_direction_reg(motor_l_pins[i]);
        _motor_r.direction_reg[i] = resolve_direction_reg(motor_r_pins[i]);
    }

    // Setup the pin numbering offsets.
    for(uint8_t i = 0; i < MOTORS_NUM; ++i)
    {
        _motor_l.pins_offset[i] = resolve_pin_offset(motor_l_pins[i]);
        _motor_r.pins_offset[i] = resolve_pin_offset(motor_r_pins[i]);
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
    uint8_t rot_dir;

    switch(direction)
    {
        case forward:
            rot_dir = 0;
            break;
        
        case reverse:
            rot_dir = 1;
            break;
        
        case stop:
            rot_dir = 0;
            speed_conv = 0;
            break;

        default:
            rot_dir = 0;
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