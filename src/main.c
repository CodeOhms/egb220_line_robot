#include "initialise.h"
#include "peripherals.h"
#include "timers.h"
#include "interrupts_timer.h"
#include "leds.h"
#include "motors.h"
#include "sensors.h"

#include <avr/interrupt.h>

#define TOLERANCE 120
#define BB_BASE 72
#define PD_BASE 55
#define BB_SLIGHT 70
#define KP 8.75

#ifdef ENV_AVR

#include <util/delay.h>

void delay(double ms)
{
    _delay_ms(ms);
}

// Global variables:
int Base;
int Type;
int error;
int position = 0;
int *last_error = 0;

double Kp;
double Kd = 1.45;
uint16_t sensor_readings[SENSORS_NUM];

#endif //ENV_AVR

int current_position()
{
    int position = 0;

    if(sensor_readings[1] < TOLERANCE  && sensor_readings[2] < TOLERANCE  && sensor_readings[3] < TOLERANCE
    && sensor_readings[4] < TOLERANCE  && sensor_readings[5] < TOLERANCE  && sensor_readings[6] < TOLERANCE)
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

void led_init(){
	DDRB |= (1<<6)|(1<<5)|(1<<2)|(1<<1);
}

void control(double kp, double kd, int *last_error, int base, int type)
{
    int current_pos = current_position(sensor_readings);
    if(type == 1){
        if(sensor_readings[3] < TOLERANCE && sensor_readings[4] < TOLERANCE) { 	// if 4 & 5 see line, go straight
            // motor_move(85, forward, MOTOR_A);
            // motor_move(85, forward, MOTOR_B);
            motor_move(BB_BASE, forward, MOTOR_A);
            motor_move(BB_BASE, forward, MOTOR_B);
        }
		else if(sensor_readings[3] >= TOLERANCE && sensor_readings[4] < TOLERANCE){	// if 5 sees the line and 4 does not SLIGHT LEFT
            // motor_move(85, forward, MOTOR_A);
            // motor_move(84, forward, MOTOR_B);
            motor_move(BB_BASE, forward, MOTOR_A);
            motor_move(BB_SLIGHT, forward, MOTOR_B);
		}
		else if(sensor_readings[3] < TOLERANCE && sensor_readings[4] >= TOLERANCE){	// if 4 sees line & 5 does not SLIGHT RIGHT
            // motor_move(84, forward, MOTOR_A);
            // motor_move(85, forward, MOTOR_B);
            motor_move(BB_SLIGHT, forward, MOTOR_A);
            motor_move(BB_BASE, forward, MOTOR_B);
		}
    }
	else{
		if(
			sensor_readings[6] > TOLERANCE
			&& sensor_readings[5] > TOLERANCE 
			&& sensor_readings[4] > TOLERANCE
			&& sensor_readings[3] > TOLERANCE 
			&& sensor_readings[2] > TOLERANCE
			&& sensor_readings[1] > TOLERANCE 
		)
	{
//			error = *last_error;
			motor_move(0, forward, MOTOR_A);
			motor_move(0, forward, MOTOR_B);
	}
        error = 0 - current_pos;
        int derivative = error - *last_error;
        int control = (kp * error) + (kd * derivative);

        motor_move(base + control, forward, MOTOR_A);
        motor_move(base - control, forward, MOTOR_B);
        *last_error = error;
	}
}



// Use this as our "int main(void)" replacement.
void line_following_robot(void){
    while(1){
        // int position = 0;

        // Sample from all sensors:
        // uint16_t sensor_readings[SENSORS_NUM];
        for(uint8_t s = 0; s < SENSORS_NUM; ++s){
            sensor_readings[s] = sensor_read(s);
        }

        position = current_position(sensor_readings);
        if(position == -1 || position == 0 || position == 1){
            Type = 1;
			PORTB |= (1<<6)|(1<<2);
			PORTB &= ~((1<<5)|(1<<1));
		}

        else{
            Type = 0;
            Base = PD_BASE;
            Kp = KP;
			PORTB &= ~((1<<6)|(1<<2));
			PORTB |= (1<<5)|(1<<1);
		}

        control(Kp, Kd, *last_error, Base, Type);

    }

    // Do not allow this function to return, otherwise `main()` will end!
}

#ifdef ENV_AVR

int main(void){
    initialise();
    line_following_robot();

    return 0;
}

#endif //ENV_AVR
