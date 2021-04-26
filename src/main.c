#include "initialise.h"
#include "peripherals.h"
#include "leds.h"
#include "motors.h"

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

        // // Test motors:
        // motor_move(10.0, reverse, MOTOR_A);
        // motor_move(10.0, reverse, MOTOR_B);
        // delay(1000);

        // motor_move(10.0, reverse, MOTOR_A);
        // motor_move(10.0, reverse, MOTOR_B);
        // delay(1000);

        // motor_move(100.0, reverse, MOTOR_A);
        // motor_move(100.0, reverse, MOTOR_B);
        // delay(1000);

        // motor_move(100.0, forward, MOTOR_A);
        // motor_move(100.0, forward, MOTOR_B);
        // delay(1000);



        // Complete track following:

        // Sample from all sensors:
        uint8_t sensor_readings[SENSORS_NUM];
        for(uint8_t s = 0; s < SENSORS_NUM; ++s)
        {
            sensor_readings[s] = sensor_read();
        }

        // Control logic:
        if (sensor_readings[3] < TOLERANCE && sensor_readings[4] < TOLERANCE) { // If 4 & 5 see line, go straight
			motor_move(47.058823, forward, MOTOR_A);
			motor_move(47.058823, forward, MOTOR_B);
		}		
		else if(sensor_readings[3] >= TOLERANCE && sensor_readings[4] < TOLERANCE){ // if 5 sees line & 4 does not veer left
			motor_move(47.058823, forward, MOTOR_A);
			motor_move(43.137254, forward, MOTOR_B);
		}
		else if(sensor_readings[3] < TOLERANCE && sensor_readings[4] >= TOLERANCE){ // if 4 sees line & 5 does not veer right
			motor_move(43.137254, forward, MOTOR_A);
			motor_move(47.058823, forward, MOTOR_B);
		}
		else if(sensor_readings[5] < TOLERANCE && sensor_readings[2] < TOLERANCE){ // if 3 & 6 see line go straight (intersection)
			motor_move(47.058823, forward, MOTOR_A);
			motor_move(47.058823, forward, MOTOR_B);
		}
		else if(sensor_readings[2] < TOLERANCE && sensor_readings[5] >= TOLERANCE){ // if 3 sees line & 6 does not, stronger veer right
			motor_move(31.372549, forward, MOTOR_A);
			motor_move(43.137254, forward, MOTOR_B);
		}
		else if(sensor_readings[5] < TOLERANCE && sensor_readings[2] >= TOLERANCE){ // if 6 sees line & 3 does not, stronger veer left
			motor_move(43.137254, forward, MOTOR_A);
			motor_move(31.372549, forward, MOTOR_B);
		}
		else if(sensor_readings[6] < TOLERANCE && sensor_readings[1] >= TOLERANCE){ // if 7 sees line & 2 does not, stop left motor, slow right motor (turn right)
			motor_move(39.215686, forward, MOTOR_A);
			motor_move(0.0, forward, MOTOR_B);
		}
		else if(sensor_readings[1] < TOLERANCE && sensor_readings[6] >= TOLERANCE){ // if 2 sees line & 7 does not, stop right motor, slow left motor (turn left)
			motor_move(0.0, forward, MOTOR_A);
			motor_move(39.215686, forward, MOTOR_B);
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