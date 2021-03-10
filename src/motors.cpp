#include "include/motors.h"


// 0th index is for phase and 1st index is for enable pin on H bridge.
uint8_t _motor_l_pins[2] = 0;
uint8_t _motor_r_pins[2] = 0;

void motor_init(uint8_t* motor_l_pins, uint8_t* motor_r_pins)
{
    // Copy the arrays.
    for(uint8_t i = 0; i < 2; ++i)
    {
        _motor_l_pins[i] = motor_l_pins[i];
        _motor_r_pins[i] = motor_r_pins[i];
    }

    // Setup output pins for controlling the motors.
        // Do not need to setup the pins for output mode if using analogWrite().
    pinMode(_motor_l_pins[1], OUTPUT);
    pinMode(_motor_r_pins[1], OUTPUT);
}

void motor_close()
{
    // Clear the arrays.
    for(uint8_t i = 0; i < 2; ++i)
    {
        _motor_l_pins[i] = 0;
        _motor_r_pins[i] = 0;
    }
}

uint8_t speed_to_pwm_value(float speed)
{
    if(!(speed >= 0 && speed =< 100))
    {
        return 0;
    }

    //100%:255 divide both by 100 --> 1%:2.55. Conv to int is needed.
    uint8_t speed_conv = speed * 2.55; // Will concatenate.

    return speed_conv;
}

void motor_move_direct(float speed, enum motor_direction direction, int enable_pin, int phase_pin)
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
    digitalWrite(phase_pin, rot_dir);
        // Setup pwm signal.
    analogWrite(enable_pin, speed_conv);
}

void motor_move(float speed, enum motor_direction direction, enum motors)
{
    uint8_t* motor_selected = 0;
    switch(motors)
    {
        case motor_left:
            motor_selected = _motor_l_pins;
            break;
        
        case motor_right:
            motor_selected = _motor_r_pins;
            break;
    }

    motor_move_direct(speed, direction, motor_selected[1], motor_selected[0]);
}