#ifdef ENV_AVR

#include "peripherals.h"
#include "initialise.h"
#include "main.h"
#include "timers.h"
#include "interrupts_timer.h"
#include "leds.h"
#include "motors.h"
#include "sensors.h"

void initialise(void)
{
    // Start motors and pwm:
        // Start pwm:
            // Set up fast pwm with default TOP of 0xFF (255):
    timer0_waveform_generation_mode(fast_pwm_0xFF);
            // Select prescaler:
    timer0_select_prescaler(timer_prescaler256);

        // Setup motors with non-inverting pwm mode
            // Motor A:
        timer0_compare_output_mode(com_clear, comp_A);
            // Motor B:
        timer0_compare_output_mode(com_clear, comp_B);
        // Start motors:
    motor_init();

    // Start leds:
    led_init();

    // Start sensors (starts ADC too):
    sensor_init();
}

#endif //ENV_AVR