#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>

#include "fast_pwm.h"

         uint8_t _fast_pwm_is_initialised = 0;
volatile uint8_t** _compare_regs = 0; // Pointer to array.

void pwm_inverted(enum pins_mcu pin_mcu)
{
    uint8_t offsets_X[2];

    // Resolve the needed offsets:
    read_pwm_inverting_mode_bits_offset(pin_mcu, offsets_X);

    TCCR0A |= (1<<offsets_X[1]);
    TCCR0A |= (1<<offsets_X[0]);
}

void pwm_non_inverted(enum pins_mcu pin_mcu)
{
    uint8_t offsets_X[2];

    // Resolve the needed offsets:
    read_pwm_inverting_mode_bits_offset(pin_mcu, offsets_X);

    TCCR0A |= (1<<offsets_X[1]);
    TCCR0A &= ~(1<<offsets_X[0]);
}

uint8_t fast_pwm_init(enum prescalers prescaler, uint8_t pwm_is_inverted, uint8_t* compare_thresholds,
                      enum pins_mcu* pins, uint8_t num_pins)
{
    // Set up fast pwm with default TOP of 0xFF (255):
    TCCR0A |= (1<<WGM00);
    TCCR0A |= (1<<WGM01);
    TCCR0A &= ~(1<<WGM02);

    // Select prescaler:
    fast_pwm_select_prescaler(prescaler);    

    // Allocate memory for the array of compare registers:
    _compare_regs = (volatile uint8_t**) malloc(num_pins * sizeof(_compare_regs[0]));

    // Resolve used compare registers, set pwm inverting mode:
    for(uint8_t i = 0; i < num_pins; ++i)
    {
        enum pins_mcu pin = pins[i];
        if(exists_compare_reg(pin))
        {
            _compare_regs[i] = resolve_compare_reg(pin);

            // Inverted pwm signal?
            if(pwm_is_inverted)
            {
                pwm_inverted(pin);
            }
            else
            {
                pwm_non_inverted(pin);
            }
        }
        else
        {
            // TODO: compare register doesn't exist. Fatal error!
            return 0;
        }
    }

    // Set compare thresholds using the corresponding registers:
    for(uint8_t i = 0; i < num_pins; ++i)
    {
        fast_pwm_set_compare_counter(pins[i], compare_thresholds[i]);
    }

    // User code must set PWM pins as outputs!

    _fast_pwm_is_initialised = 1;

    return 1;
}

void fast_pwm_close()
{
    // Select no prescaler:
    fast_pwm_select_prescaler(no_prescaler);

    // Reset compare thresholds to zero:
    for(uint8_t i = 0; i < sizeof(_compare_regs)/sizeof(compare_registers[0]); ++i)
    {
        fast_pwm_set_compare_counter_direct(i, 0);
    }
    
    // Disable fast pwm mode (set all to zero):
    TCCR0A &= ~(1<<WGM00);
    TCCR0A &= ~(1<<WGM01);
    TCCR0A &= ~(1<<WGM02);

    // Free memory:
    free((uint8_t*) _compare_regs); // Cast to avoid the annoying warning.

    _fast_pwm_is_initialised = 0;
}

void fast_pwm_select_prescaler(enum prescalers prescaler)
{
    switch(prescaler)
    {
    case no_clock_source:
        TCCR0B &= ~(1<<CS02);
        TCCR0B &= ~(1<<CS01);
        TCCR0B &= ~(1<<CS00);
        break;
    
    case no_prescaler:
        TCCR0B &= ~(1<<CS02);
        TCCR0B &= ~(1<<CS01);
        TCCR0B |= (1<<CS00);
        break;
    
    case prescaler8:
        TCCR0B &= ~(1<<CS02);
        TCCR0B |= (1<<CS01);
        TCCR0B &= ~(1<<CS00);
        break;
    
    case prescaler64:
        TCCR0B &= ~(1<<CS02);
        TCCR0B |= (1<<CS01);
        TCCR0B |= (1<<CS00);
        break;
    
    case prescaler256:
        TCCR0B |= (1<<CS02);
        TCCR0B &= ~(1<<CS01);
        TCCR0B &= ~(1<<CS00);
        break;
    
    case prescaler1024:
        TCCR0B |= (1<<CS02);
        TCCR0B &= ~(1<<CS01);
        TCCR0B |= (1<<CS00);
        break;
    
    case extern_clock_falling:
        TCCR0B |= (1<<CS02);
        TCCR0B |= (1<<CS01);
        TCCR0B &= ~(1<<CS00);
        break;
    
    case extern_clock_rising:
        TCCR0B |= (1<<CS02);
        TCCR0B |= (1<<CS01);
        TCCR0B |= (1<<CS00);
        break;
    
    default:
        // Something went wrong!
        break;
    }
}

void fast_pwm_set_compare_counter(enum pins_mcu pin_mcu, uint8_t compare_threshold)
{
    // uint8_t index = read_compare_reg_index(pin_mcu);
    volatile uint8_t* compare_register = resolve_compare_reg(pin_mcu);
    // fast_pwm_set_compare_counter_direct(index, compare_threshold);
    *(compare_register) = compare_threshold;
}