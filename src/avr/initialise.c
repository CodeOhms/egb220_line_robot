#ifdef ENV_AVR

#include "peripherals.h"
#include "initialise.h"
#include "main.h"
#include "interrupts_timer.h"
#include "fast_pwm.h"
#include "leds.h"
#include "motors.h"

void initialise(void)
{
    // Set up motors and pwm.
        // Set up pwm.
    fast_pwm_init(prescaler256, 0);

        // Set up motors.
    motor_init();

    // Set up leds.
    led_init();
}

#endif //ENV_AVR