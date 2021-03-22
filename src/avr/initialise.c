#ifdef ENV_AVR

#include "peripherals.h"
#include "pins.h"
#include "initialise.h"
#include "main.h"
#include "interrupts_timer.h"
#include "fast_pwm.h"
#include "leds.h"
#include "motors.h"

void initialise(void)
{
    pins_init();

    // Set up leds.
    // enum pins_mcu led_pins[8] = { PE_Pin6, PB_Pin0, PB_Pin1, PB_Pin2, PB_Pin7, PD_Pin0, PB_Pin6, PB_Pin5 };
    // led_init(led_pins);

    // Set up motors and pwm.
    // enum pins_mcu motor_l_pins[HBRIDGE_PINS_PER_M] = { PB_Pin0, PB_Pin7 }; // Motor A.
    // enum pins_mcu motor_l_pins[HBRIDGE_PINS_PER_M] = { PC_Pin6, PB_Pin7 }; // Motor A pro micro testing ONLY!
    // enum pins_mcu motor_r_pins[HBRIDGE_PINS_PER_M] = { PE_Pin6, PD_Pin0 }; // Motor B.
    enum pins_mcu motors_pins[MOTORS_NUM][HBRIDGE_PINS_PER_M] = {
        { PC_Pin6, PB_Pin7 }, // Motor A, pro micro testing ONLY!
        { PE_Pin6, PD_Pin0 } //Motor B. 
    };

        // Set up pwm.
    enum pins_mcu motors_enable_pins[] = { motors_pins[(uint8_t) motor_left][ENABLE], motors_pins[(uint8_t) motor_right][ENABLE] };
    uint8_t compare_thresholds[] = { 0, 0 };
    fast_pwm_init(prescaler256, 0, compare_thresholds, motors_enable_pins, 2);
        // Set up motors.
    motor_init(motors_pins);
}

#endif //ENV_AVR