#include <stdlib.h>

#include "interrupts.h"
#include "fast_pwm.h"

uint8_t _num_timing_counters = 0;
uint8_t* _timing_counters = 0;
uint8_t* _counters_limits  = 0;
// void (**_pwm_user_functions)(void) = 0;
func_ptr_rvoid_t* _pwm_user_functions = 0;
uint8_t* _user_functions_enabled = 0;

void fast_pwm_init_min(uint8_t num_timing_counters)
{
    _num_timing_counters = num_timing_counters;

    // Enable overflow interrupt.
    TIMSK0 |= (1<<0);
    // Select a prescaler of 256.
    TCCR0B |=  (1<<2);
    TCCR0B &= ~(1<<1);
    TCCR0B &= ~(1<<0);

    // Create the arrays and init to 0 (calloc inits all to 0).
    _timing_counters = calloc(num_timing_counters, sizeof(uint8_t));
    _counters_limits  = calloc(num_timing_counters, sizeof(uint8_t));
    _pwm_user_functions = calloc(num_timing_counters, sizeof(func_ptr_rvoid_t));
    _user_functions_enabled = calloc(num_timing_counters, sizeof(uint8_t));
}

void fast_pwm_init(uint8_t num_timing_counters, uint8_t* counters_limits, func_ptr_rvoid_t* pwm_user_functions, uint8_t* user_functions_enabled)
{
    fast_pwm_init_min(num_timing_counters);

    // Copy the arrays.
    for(uint8_t i = 0; i < num_timing_counters; ++i)
    {
        _pwm_user_functions[i] = pwm_user_functions[i];
        _counters_limits[i] = counters_limits[i];
        _user_functions_enabled[i] = user_functions_enabled[i];
    }
}

void fast_pwm_close()
{
    _num_timing_counters = 0;

    // Disable overflow interrupt.
    TIMSK0 &= ~(1<<0);
    // Do NOT touch the prescaler, just in case something else is using it.
    // Free memory.
    free(_timing_counters);
    free(_counters_limits);
    free(_pwm_user_functions);
    free(_user_functions_enabled);
}

void fast_pwm_set_counter(uint8_t counter_index, uint8_t counter_limit)
{
    _counters_limits[counter_index] = counter_limit;
}

uint8_t fast_pwm_set_data(uint8_t counter_index, uint8_t counter_limit,
                       func_ptr_rvoid_t function,
                       uint8_t function_enabled)
{
    if(counter_index < 0 || counter_index +1 > _num_timing_counters)
    { // Index out of range!
        return 1;
    }

    _counters_limits[counter_index] = counter_limit;
    _pwm_user_functions[counter_index] = function;
    if(function_enabled > 2)
    { // This must be a boolean (either 0 or 1).
        return 2;
    }
    _user_functions_enabled[counter_index] = function_enabled;

    return 0;
}

ISR(TIMER0_OVF_vect)
{
    // Check for timing trigger.
    for(uint8_t i = 0; i < _num_timing_counters; ++i)
    {
        if(!_user_functions_enabled[i]) // Not enabled.
        {
            continue;
        }

        uint8_t* counter = &_timing_counters[i];
        uint8_t  limit = _counters_limits[i];
        ++*counter;
        if(*counter >= limit)
        {
            // Call the associated function.
            (*_pwm_user_functions[i])();
            // Reset the counter.
            *counter = 0;
        }
    }
}