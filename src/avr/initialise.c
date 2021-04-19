#ifdef ENV_AVR

#include "peripherals.h"
// #include "pins.h"
#include "initialise.h"
#include "main.h"
#include "interrupts_timer.h"
#include "fast_pwm.h"
// #include "leds.h"
#include "motors.h"

void initialise(void)
{
    // pins_init();

    // Set up leds.
    // enum pins_mcu led_pins[8] = { PE_Pin6, PB_Pin0, PB_Pin1, PB_Pin2, PB_Pin7, PD_Pin0, PB_Pin6, PB_Pin5 };
    // led_init(led_pins);

    // Set up motors and pwm.
        // Set up pwm.
    fast_pwm_init(prescaler256, 0);
        // Set up motors.
    motor_init();
}

#endif //ENV_AVR