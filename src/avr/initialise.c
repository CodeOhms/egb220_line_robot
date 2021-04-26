#ifdef ENV_AVR

#include "peripherals.h"
#include "initialise.h"
#include "main.h"
#include "interrupts_timer.h"
#include "fast_pwm.h"
#include "leds.h"
#include "motors.h"
#include "sensors.h"

void initialise(void)
{
    // Start motors and pwm:
        // Start pwm:
    fast_pwm_init(f_pwm_prescaler256, 0);

        // Start motors:
    motor_init();

    // Start leds:
    led_init();

    // Start sensors (starts ADC too):
    sensor_init();
}

#endif //ENV_AVR