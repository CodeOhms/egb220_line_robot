#ifndef  INTERRUPTS_TIMER_H
#define  INTERRUPTS_TIMER_H

#define BOOTLOADER_CATERINA 0
#define BOOTLOADER_DFU 1

#include <stdint.h>
#include <stdlib.h>

/*
Needs three arrays with corresponding indexes.
    `num_timing_counters`: an array of counters for timing the individual pwm
signals.
    `counters_limits`: an array for storing the count limit for when the 
pwm interrupt needs to trigger the corresponding command of the counter.
    `pwm_user_functions`: an array for the associated functions of each
pwm signal (also known as a function table).
E.g. counter 0 toggles an led and counter 1 toggles an enable
pin on the h at 40% duty cycle, or every 102 counts (0.4 * 255).
*/
typedef void(*func_ptr_rvoid_t)(void);
// void pwm_init(uint8_t num_timing_counters, void (**pwm_user_functions)(void));
void interrupts_init(uint8_t num_timing_counters, uint8_t* counters_limits,
                   func_ptr_rvoid_t* pwm_user_functions,
                   uint8_t* user_functions_enabled);
// Provide alt init function to set counter limits and user functions later.
void interrupts_init_min(uint8_t num_timing_counters);

void interrupts_close();

void interrupts_set_counter(uint8_t counter_index, uint8_t counter_limit);

/*
Changes the corresponding values for a specific set of counter info
using the given index.
Call the `interrupts_init_min()` function first!
*/
uint8_t interrupts_set_data(uint8_t counter_index, uint8_t counter_limit,
                         func_ptr_rvoid_t function,
                        uint8_t function_enabled);

uint8_t interrupts_func_is_enabled(uint8_t function_index);

double get_current_time();

#endif //INTERRUPTS_TIMER_H