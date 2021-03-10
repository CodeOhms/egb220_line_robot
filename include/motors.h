#ifndef   MOTORS_H
#define   MOTORS_H

#include <Arduino.h>


void motor_init();

void motor_move(float speed, int pwm_pin1, int pwm_pin2);


#endif //MOTORS_H