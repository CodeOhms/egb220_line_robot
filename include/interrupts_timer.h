#ifndef  INTERRUPTS_TIMER_H
#define  INTERRUPTS_TIMER_H

#define BOOTLOADER_CATERINA 0
#define BOOTLOADER_DFU 1

#include <stdint.h>
#include <stdlib.h>

#include "timers.h"

/*
Needs three arrays with corresponding indexes.
    `num_overflow_counters`: an array of counters for timing the individual pwm
signals.
    `time_limits`: an array for storing the time limits (in ms) for when the 
timer interrupt needs to trigger the corresponding command of the counter.
    `timer_user_functions`: an array for the associated functions of each
timer (also known as a function table).
*/
typedef void(*func_ptr_rvoid_t)(void);
void interrupts_timer_init(uint8_t num_overflow_counters, double* time_limits,
                     func_ptr_rvoid_t* timer_user_functions,
                     uint8_t* user_functions_enabled);
// Provide alt init function to set counter limits and user functions later.
void interrupts_timer_init_min(uint8_t num_overflow_counters);

void interrupts_timer_close();

void interrupts_timer_set_counter(uint8_t counter_index, uint8_t counter_limit);

/*
Changes the corresponding values for a specific set of counter info
using the given index.
Call the `interrupts_timer_init_min()` function first!
*/
uint8_t interrupts_timer_set_data(uint8_t counter_index, uint8_t counter_limit,
                         func_ptr_rvoid_t function,
                        uint8_t function_enabled);

uint8_t interrupts_timer_func_is_enabled(uint8_t function_index);

double counter_to_time_t0();

uint32_t time_to_counter_8bit(double time_ms, double timer_prescaler);

#endif //INTERRUPTS_TIMER_H