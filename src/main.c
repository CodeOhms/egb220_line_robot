#include "initialise.h"
#include "peripherals.h"
#include "leds.h"
#include "motors.h"

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
        // Test LEDs:
            // Turn on:
        led_on(LED0); // on
        led_on(LED1); // on
        led_toggle(LED2); // on
        delay(1000);
            // Turn off:
        led_off(LED0); // off
        led_toggle(LED1); // off
        led_toggle(LED2); // off

        // Test motors:
        motor_move(10.0, reverse, MOTOR_A);
        motor_move(10.0, reverse, MOTOR_B);
        delay(1000);

        motor_move(10.0, reverse, MOTOR_A);
        motor_move(10.0, reverse, MOTOR_B);
        delay(1000);

        motor_move(100.0, reverse, MOTOR_A);
        motor_move(100.0, reverse, MOTOR_B);
        delay(1000);

        motor_move(100.0, forward, MOTOR_A);
        motor_move(100.0, forward, MOTOR_B);
        delay(1000);
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