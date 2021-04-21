#ifndef  FAST_PWM_H
#define  FAST_PWM_H

/*
PWM resoultion of 256 values (0 -> 255 inclusive).
*/

extern uint8_t _fast_pwm_is_initialised;

enum pwm_prescalers
{
    no_clock_source,
    no_prescaler,
    prescaler8,
    prescaler64,
    prescaler256,
    prescaler1024,
    extern_clock_falling,
    extern_clock_rising
};

/*
Initialises the fast pwm functionality.

Needs:
- A `prescaler` option selected.
- A "boolean" value set for pwm_inverted to choose either inverting or non-inverting pwm
    (compare match sets pwm signal low).
- The array `compare_thresholds` is to set the compare match threshold.
- An array of pwm enabled pins that will be used.
- The number of pwm pins, or the size of the `pins` array.

NOTE: this will not setup the pins as outputs, caller code must do this!
*/
// uint8_t fast_pwm_init(enum pwm_prescalers prescaler, uint8_t pwm_inverted, uint8_t* compare_thresholds,
//                       enum pins_mcu* pins, uint8_t num_pins);
uint8_t fast_pwm_init(enum pwm_prescalers prescaler, uint8_t pwm_inverted);

void fast_pwm_close();

void fast_pwm_select_prescaler(enum pwm_prescalers prescaler);

// void fast_pwm_set_compare_counter(enum pins_mcu pin_mcu, uint8_t compare_threshold);

#endif //FAST_PWM_H