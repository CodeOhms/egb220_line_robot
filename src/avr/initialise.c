#if ENV_AVR == 1

#include "initialise.h"
#include "main.h"
#include "leds.h"
#include "motors.h"


void initialise(void)
{
    // Set up leds.
    uint8_t led_pins[8] = { PE6, PB0, PB1, PB2, PB7, PD0, PB6, PB5 };
    led_init(led_pins);

    // Set up motors.
    uint8_t motor_l_pins[2] = { PB0, PB7 }; // Motor A.
    uint8_t motor_r_pins[2] = { PE6, PD0 }; // Motor B.
    // NOTE: seems like there are leds tied to each pin controlling the H bridge...
    motor_init(motor_l_pins, motor_r_pins);
}

#endif //ENV_AVR