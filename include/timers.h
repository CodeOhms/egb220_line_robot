#ifndef TIMERS_H 
#define TIMERS_H

#include <avr/io.h>

enum timer_prescalers
{
    no_clock_source,
    no_prescaler,
    timer_prescaler8,
    timer_prescaler64,
    timer_prescaler256,
    timer_prescaler1024,
    extern_clock_falling,
    extern_clock_rising
};

enum waveform_generation_mode
{
    normal,
    pwm_phase_correct_0xFF,
    CTC,
    fast_pwm_0xFF,
    pwm_phase_correct_OCRx,
    fast_pwm_OCRx
};

enum compare_output_mode
{
    com_normal,
    com_toggle,
    com_clear,
    com_set
};

enum compare_output
{
    comp_A,
    comp_B,
    comp_C
};

void timer0_set_prescaler(enum timer_prescalers prescaler);

enum timer_prescalers timer0_get_prescaler();

uint16_t timer_prescaler_enum_to_int(enum timer_prescalers prescaler);

void timer0_waveform_generation_mode(enum waveform_generation_mode wgm);

void timer0_compare_output_mode(enum compare_output_mode cop, enum compare_output comp_out);

#endif //TIMERS_H