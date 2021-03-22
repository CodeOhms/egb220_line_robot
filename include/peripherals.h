#ifndef  PERIPHERALS_H
#define  PERIPHERALS_H

enum leds
{
    led0,
    led1,
    led2,
    led3,
    led4,
    led5,
    led6,
    led7
};

enum motors
{
    motor_left,
    motor_right
};

enum pwm_devices
{
    pwm_motor_left,
    pwm_motor_right
};

#define PWM_NUM_DEVICES 2

#endif //PERIPHERALS