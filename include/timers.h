#ifndef TIMERS_H 
#define TIMERS_H

#include <avr/io.h>

enum timer_prescalers
{
    no_clock_source,
    no_prescaler,
    f_pwm_prescaler8,
    f_pwm_prescaler64,
    f_pwm_prescaler256,
    f_pwm_prescaler1024,
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
    normal,
    toggle,
    clear,
    set
};

enum compare_output
{
    comp_A,
    comp_B,
    comp_C
};

void timer0_select_prescaler(enum timer_prescalers prescaler);

void timer0_waveform_generation_mode(enum waveform_generation_mode wgm);

void timer0_compare_output_mode(enum compare_output_mode cop, enum compare_output comp_out);

#endif //TIMERS_H