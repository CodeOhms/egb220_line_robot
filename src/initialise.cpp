#include "include/initialise.h"

#ifdef    ENV_ARDUINO
#include <Arduino.h>
#include "include/main.h"

void setup(void)
{
    // Leave this empty so we can use the 'initialise(void)' function instead.
}

void initialise(void)
{
    // Set up leds.
    uint8_t led_pins[8] = { PE6, PB0, PB1, PB2, PB7, PD0, PB6, PB5 };
    led_init(led_pins);

    // Set up motors.
    uint8_t motor_l_pins[2] = { PB0, OC0A }; // Motor A.
    uint8_t motor_r_pins[2] = { PE6, OC0B }; // Motor B.
    // NOTE: seems like led1 is tied to motor A and led0 is tied to B...
    motor_init(moto_l_pins, motor_r_pins);
}

/*
Have this function pass control over to 'line_following_robot(void)' function.
This function should never pass control back.
*/
void loop(void)
{
    line_following_robot();
}

#endif //ENV_ARDUINO


#ifdef   ENV_AVR

void initialise(void)
{

}

#endif //ENV_AVR