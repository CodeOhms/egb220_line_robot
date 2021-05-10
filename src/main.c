#include "initialise.h"
#include "peripherals.h"
#include "leds.h"
#include "motors.h"
#include "sensors.h"

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

// Use this as our "int main(void)" replacement.
void line_following_robot(void)
{
    while(1)
    {
        // // Test LEDs:
        //     // Turn on:
        // led_on(LED0); // on
        // led_on(LED1); // on
        // led_toggle(LED2); // on
        // delay(1000);
        //     // Turn off:
        // led_off(LED0); // off
        // led_toggle(LED1); // off
        // led_toggle(LED2); // off

        // Test motors:
        // motor_move(10.0, reverse, MOTOR_A);
        // motor_move(10.0, reverse, MOTOR_B);
        // delay(1000);

        // motor_move(10.0, forward, MOTOR_B);
        // motor_move(10.0, forward, MOTOR_A);
        // delay(1000);

        // motor_move(40.0, reverse, MOTOR_A);
        // motor_move(40.0, reverse, MOTOR_B);
        // delay(1000);

        // motor_move(40.0, forward, MOTOR_A);
        // motor_move(40.0, forward, MOTOR_B);
        // delay(1000);



        // Complete track following:

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

    // Do not allow this function to return!
}

#ifdef ENV_AVR

int main(void)
{
    initialise();
    line_following_robot();

    return 0;
}

#endif //ENV_AVR