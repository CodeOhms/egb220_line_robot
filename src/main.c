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
#define TOLERANCE 125

#ifdef ENV_AVR

#include <util/delay.h>

void delay(double ms)
{
    _delay_ms(ms);
}

#endif //ENV_AVR

void blink()
{
	PORTD ^= (1<<PORTD5);
}

// Use this as our "int main(void)" replacement.
void line_following_robot(void)
{
	// Blink the blue LED once per second:
	DDRD |= (1<<PORTD5);
	double tim_lim[1] = { 1000 };
	func_ptr_rvoid_t func[1] = { blink };
	// void (*func[1]) = { blink };
	uint8_t en[1] = { 1 };
	interrupts_init(1, tim_lim, func, en);

    while(1)
    {
        // Sample from all sensors:
        uint16_t sensor_readings[SENSORS_NUM];
        for(uint8_t s = 0; s < SENSORS_NUM; ++s)
        {
            sensor_readings[s] = sensor_read(s);
        }

        // Control logic:
        if (sensor_readings[3] < TOLERANCE && sensor_readings[4] < TOLERANCE) { 	// if 4 & 5 see line, go straight
			motor_move(BASE_PERCENT, forward, MOTOR_A);
			motor_move(BASE_PERCENT, forward, MOTOR_B);
		}		
		else if(sensor_readings[3] >= TOLERANCE && sensor_readings[4] < TOLERANCE){	// if 5 sees the line and 4 does not SLIGHT LEFT
			motor_move(SLIGHT_PERCENT, forward, MOTOR_A);
			motor_move(BASE_PERCENT, forward, MOTOR_B);
		}
		else if(sensor_readings[3] < TOLERANCE && sensor_readings[4] >= TOLERANCE){	// if 4 sees line & 5 does not SLIGHT RIGHT
			motor_move(BASE_PERCENT, forward, MOTOR_A);
			motor_move(SLIGHT_PERCENT, forward, MOTOR_B);
		}
		else if(sensor_readings[5] < TOLERANCE && sensor_readings[2] < TOLERANCE){	// if 3 & 6 see line go straight (intersection)
			motor_move(BASE_PERCENT, forward, MOTOR_A);
			motor_move(BASE_PERCENT, forward, MOTOR_B);
		}
		else if(sensor_readings[2] < TOLERANCE && sensor_readings[5] >= TOLERANCE){	// if 3 sees line & 6 does not, MID RIGHT
			motor_move(BASE_PERCENT, forward, MOTOR_A);
			motor_move(0.0, forward, MOTOR_B);
		}
		else if(sensor_readings[5] < TOLERANCE && sensor_readings[2] >= TOLERANCE){	// if 6 sees line & 3 does not, MID LEFT
			motor_move(0.0, forward, MOTOR_A);
			motor_move(BASE_PERCENT, forward, MOTOR_B);
        }
		else{
			motor_move(0.0, forward, MOTOR_A);
			motor_move(0.0, forward, MOTOR_B);
		}
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