#include "interrupts_timer.h"

#include <avr/interrupt.h>

volatile uint8_t _num_overflow_counters = 0;
volatile uint32_t* _overflow_counters = NULL;
volatile uint32_t* _time_limits  = NULL;
volatile func_ptr_rvoid_t* _timer_user_functions = NULL;
volatile uint8_t* _user_functions_enabled = NULL;

void interrupts_init_min(uint8_t num_overflow_counters)
{    
    _num_overflow_counters = num_overflow_counters;

    // Enable overflow interrupt.
    TIMSK0 |= (1<<TOIE0);
    // Do NOT touch the prescaler, just in case something else is using it.

    // Allocate the arrays:
    _overflow_counters = malloc(num_overflow_counters * sizeof(uint32_t));
    _time_limits  = malloc(num_overflow_counters * sizeof(uint32_t));
    _timer_user_functions = malloc(num_overflow_counters * sizeof(func_ptr_rvoid_t));
    _user_functions_enabled = malloc(num_overflow_counters * sizeof(uint8_t));

    #if BOOTLOADER == 0
    // Stop USB triggering interrupt. Left as on by Caterina bootloader :(.
    USBCON = 0;
    #endif

    sei();
}

void interrupts_init(uint8_t num_overflow_counters, double* time_limits, func_ptr_rvoid_t* timer_user_functions,
                     uint8_t* user_functions_enabled)
{
    interrupts_init_min(num_overflow_counters);

    // Copy the arrays:
    for(uint8_t i = 0; i < num_overflow_counters; ++i)
    {
        _timer_user_functions[i] = timer_user_functions[i];
        _user_functions_enabled[i] = user_functions_enabled[i];

    // Init counters to zero:
        _overflow_counters[i] = 0;

    // Convert time limits, in ms, to ticks:
        // double prescaler_number = (double) timer_prescaler_enum_to_int(timer0_get_prescaler());
        double prescaler_number = (double) 256;
        _time_limits[i] = time_to_counter_8bit(time_limits[i], prescaler_number);
    }
}

void interrupts_close()
{
    _num_overflow_counters = 0;

    // Disable overflow interrupt.
    TIMSK0 &= ~(1<<0);
    // Do NOT touch the prescaler, just in case something else is using it.
    // Free memory.
    free(_overflow_counters);
    free(_time_limits);
    free(_timer_user_functions);
    free(_user_functions_enabled);
}

void interrupts_set_counter(uint8_t counter_index, uint8_t counter_limit)
{
    _time_limits[counter_index] = counter_limit;
}

uint8_t interrupts_set_data(uint8_t counter_index, uint8_t counter_limit,
                       func_ptr_rvoid_t function,
                       uint8_t function_enabled)
{
    if(counter_index < 0 || counter_index +1 > _num_overflow_counters)
    { // Index out of range!
        return 1;
    }

    _time_limits[counter_index] = counter_limit;
    _timer_user_functions[counter_index] = function;
    if(function_enabled > 2)
    { // This must be a boolean (either 0 or 1).
        return 2;
    }
    _user_functions_enabled[counter_index] = function_enabled;

    return 0;
}

ISR(TIMER0_OVF_vect)
{
    for(uint8_t i = 0; i < _num_overflow_counters; ++i)
    {
        ++(_overflow_counters[i]);

        if(!_user_functions_enabled[i]) // Not enabled.
        {
            continue;
        }

    // Check for timing trigger, if so run function:
        if((_overflow_counters[i] * 256 + TCNT0) >= _time_limits[i])
        {
            // Call the associated function:
            (*_timer_user_functions[i])();
            // Reset the counter:
            _overflow_counters[i] = 0;
        }
    }
}

double counter_to_time_t0(uint32_t overflow_count, double timer_prescaler)
{
    return ( overflow_count * 256.0 + TCNT0 ) * timer_prescaler/((double) F_CPU);
}

uint32_t time_to_counter_8bit(double time_ms, double timer_prescaler)
{
    // Convert the milliseconds to seconds in formula!
    return (time_ms * 0.001) * ((double) F_CPU)/timer_prescaler;
}
