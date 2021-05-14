#include "initialise.h"
#include "peripherals.h"
#include "timers.h"
#include "interrupts_timer.h"
#include "leds.h"
#include "motors.h"
#include "sensors.h"

#include <avr/interrupt.h>

#define BASE 	100
#define SLIGHT 	95
#define MID		90 	
#define BASE_PERCENT ((float) BASE/2.55)
#define SLIGHT_PERCENT ((float) SLIGHT/2.55)
#define TOLERANCE 180

#ifdef ENV_AVR

#include <util/delay.h>

void delay(double ms)
{
    _delay_ms(ms);
}

// Global variables:
int Base;
int BangBang;
int error = 0;
int position = 0;
int *last_error = 0;
double Kp;
double Kd = 0.95;
uint16_t sensor_readings[SENSORS_NUM];

#endif //ENV_AVR

int current_position()
{
    int position = 0;

    if(sensor_readings[0] < TOLERANCE && sensor_readings[1] < TOLERANCE  && sensor_readings[2] < TOLERANCE  && sensor_readings[3] < TOLERANCE
    && sensor_readings[4] < TOLERANCE  && sensor_readings[5] < TOLERANCE  && sensor_readings[6] < TOLERANCE  && sensor_readings[7] < TOLERANCE)
    { // Straight line case
        position = 0;
    }
    if(sensor_readings[3] < TOLERANCE && sensor_readings[4] < TOLERANCE)
    { // Straight line case
        position = 0;
    }
    
    if(sensor_readings[1] < TOLERANCE && sensor_readings[2] >= TOLERANCE && sensor_readings[3] >= TOLERANCE && sensor_readings[4] >= TOLERANCE && sensor_readings[5] >= TOLERANCE && sensor_readings[6] >= TOLERANCE)
    {
        position =-5;
    }
    if(sensor_readings[1] < TOLERANCE && sensor_readings[2] <  TOLERANCE && sensor_readings[3] >= TOLERANCE && sensor_readings[4] >= TOLERANCE && sensor_readings[5] >= TOLERANCE && sensor_readings[6] >= TOLERANCE)
    {
        position =-4;
    }
    if(sensor_readings[2] < TOLERANCE && sensor_readings[1] >= TOLERANCE && sensor_readings[3] >= TOLERANCE && sensor_readings[4] >= TOLERANCE && sensor_readings[5] >= TOLERANCE && sensor_readings[6] >= TOLERANCE)
    {
        position =-3;
    }
    if(sensor_readings[2] < TOLERANCE && sensor_readings[3] <  TOLERANCE && sensor_readings[1] >= TOLERANCE && sensor_readings[4] >= TOLERANCE && sensor_readings[5] >= TOLERANCE && sensor_readings[6] >= TOLERANCE)
    {
        position =-2;
    }
    if(sensor_readings[3] < TOLERANCE && sensor_readings[1] >= TOLERANCE && sensor_readings[2] >= TOLERANCE && sensor_readings[4] >= TOLERANCE && sensor_readings[5] >= TOLERANCE && sensor_readings[6] >= TOLERANCE)
    {
        position =-1;
    }
    if(sensor_readings[4] < TOLERANCE && sensor_readings[1] >= TOLERANCE && sensor_readings[2] >= TOLERANCE && sensor_readings[3] >= TOLERANCE && sensor_readings[5] >= TOLERANCE && sensor_readings[6] >= TOLERANCE)
    {
        position = 1;
    }
    if(sensor_readings[4] < TOLERANCE && sensor_readings[5] <  TOLERANCE && sensor_readings[1] >= TOLERANCE && sensor_readings[2] >= TOLERANCE && sensor_readings[3] >= TOLERANCE && sensor_readings[6] >= TOLERANCE)
    {
        position = 2;
    }
    if(sensor_readings[5] < TOLERANCE && sensor_readings[1] >= TOLERANCE && sensor_readings[2] >= TOLERANCE && sensor_readings[3] >= TOLERANCE && sensor_readings[4] >= TOLERANCE && sensor_readings[6] >= TOLERANCE)
    {
        position = 3;
    }
    if(sensor_readings[5] < TOLERANCE && sensor_readings[6] <  TOLERANCE && sensor_readings[1] >= TOLERANCE && sensor_readings[2] >= TOLERANCE && sensor_readings[3] >= TOLERANCE && sensor_readings[4] >= TOLERANCE)
    {
        position = 4;
    }
    if(sensor_readings[6] < TOLERANCE && sensor_readings[1] >= TOLERANCE && sensor_readings[2] >= TOLERANCE && sensor_readings[3] >= TOLERANCE && sensor_readings[4] >= TOLERANCE && sensor_readings[5] >= TOLERANCE)
    {
        position = 5;
    }

  return position;
}

void control(double kp, double kd, int *last_error, int base, int type)
{
    int current_pos = current_position(sensor_readings);
    if(
        sensor_readings[6] > TOLERANCE
        && sensor_readings[5] > TOLERANCE 
        && sensor_readings[4] > TOLERANCE
        && sensor_readings[3] > TOLERANCE 
        && sensor_readings[2] > TOLERANCE
        && sensor_readings[1] > TOLERANCE 
      )
    {
        error = *last_error;
    }
    if(type == 1){
        if(sensor_readings[3] < TOLERANCE && sensor_readings[4] < TOLERANCE) { 	// if 4 & 5 see line, go straight
            // setMotorSpeeds(85, 85);
            motor_move(85, forward, MOTOR_A);
            motor_move(85, forward, MOTOR_B);
        }		
        else if(sensor_readings[3] >= TOLERANCE && sensor_readings[4] < TOLERANCE){	// if 5 sees the line and 4 does not SLIGHT LEFT
            // setMotorSpeeds(85, 84);
            motor_move(85, forward, MOTOR_A);
            motor_move(84, forward, MOTOR_B);
        }
        else if(sensor_readings[3] < TOLERANCE && sensor_readings[4] >= TOLERANCE){	// if 4 sees line & 5 does not SLIGHT RIGHT
            // setMotorSpeeds(84, 85);
            motor_move(84, forward, MOTOR_A);
            motor_move(85, forward, MOTOR_B);
        }
    }
    else{
        error = 0 - current_pos;
        int derivative = error - *last_error;
        int control = (kp * error) + (kd * derivative);
        // setMotorSpeeds(base + control, base - control);
        motor_move(base + control, forward, MOTOR_A);
        motor_move(base - control, forward, MOTOR_B);
        *last_error = error;
    }
}



// Use this as our "int main(void)" replacement.
void line_following_robot(void)
{
    while(1)
    {
        // int position = 0;

        // Sample from all sensors:
        // uint16_t sensor_readings[SENSORS_NUM];
        for(uint8_t s = 0; s < SENSORS_NUM; ++s)
        {
            sensor_readings[s] = sensor_read(s);
        }

        position = current_position(sensor_readings);
        if(position == -1 || position == 0 || position == 1)
        {
            BangBang = 1;
        }
        else
        {
            BangBang = 0;
            Base = 55;
            Kp = 5.5;
        }

        control(Kp, Kd, *last_error, Base, BangBang);
    }

    // Do not allow this function to return, otherwise `main()` will end!
}

#ifdef ENV_AVR

int main(void)
{
    initialise();
    line_following_robot();

    return 0;
}

#endif //ENV_AVR