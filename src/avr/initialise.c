#ifdef ENV_AVR

#include "initialise.h"
#include "main.h"
#include "pins.h"
#include "interrupts.h"
#include "fast_pwm.h"
#include "leds.h"
#include "motors.h"


void initialise(void)
{
    pins_mcu_init();

    // Set up interrupts and pwm.
    interrupts_init();
    fast_pwm_init();

    // Set up leds.
    enum pins_mcu led_pins[8] = { PE_Pin6, PB_Pin0, PB_Pin1, PB_Pin2, PB_Pin7, PD_Pin0, PB_Pin6, PB_Pin5 };
    led_init(led_pins);

    // Set up motors.
    enum pins_mcu motor_l_pins[2] = { PB_Pin0, PB_Pin7 }; // Motor A.
    enum pins_mcu motor_r_pins[2] = { PE_Pin6, PD_Pin0 }; // Motor B.
    // NOTE: seems like there are leds tied to each pin controlling the H bridge...
    motor_init(motor_l_pins, motor_r_pins);
}

#endif //ENV_AVR