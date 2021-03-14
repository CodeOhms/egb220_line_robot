#if ENV_ARDUINO == 1

#include <Arduino.h>
#include "initialise.h"
#include "main.h"
#include "leds.h"
#include "motors.h"


void setup(void)
{
    // Leave this empty so we can use the 'initialise(void)' function instead.
}

void initialise(void)
{
    // Set up leds.
    // uint8_t led_pins[8] = { PE6, PB0, PB1, PB2, PB7, PD0, PB6, PB5 };
    uint8_t led_pins[8] = { 7, 17, 15, 16, 11, 3, 10, 9 };
    led_init(led_pins);

    // Set up motors.
    // 0th index is for phase and 1st index is for enable pin on H bridge.
    uint8_t motor_l_pins[2] = { 17, 11 }; // Motor A.
    uint8_t motor_r_pins[2] = { 7, 3 }; // Motor B.
    // NOTE: seems like there are leds tied to each pin controlling the H bridge...
    motor_init(motor_l_pins, motor_r_pins);
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