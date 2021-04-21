#ifndef  ADC_H
#define  ADC_H

#if ENV_AVR == 1

#include <avr/io.h>

enum voltage_reference
{
    internal_off,
    AVcc,
    internal_on
};

enum auto_trigger_source
{
    free_running,
    analog_comparator,
    external_interrupt_request,
    timer0_compare_match_A,
    timer0_overflow,
    timer1_compare_match_B,
    timer1_overflow,
    timer1_capture_event,
    timer4_overflow,
    timer4_compare_match_A,
    timer4_compare_match_B,
    timer4_compare_match_D,
};

enum adc_prescalers
{
    prescaler2,
    prescaler4,
    prescaler8,
    prescaler64,
    prescaler128
};

void adc_on();

void adc_off();

void adc_start();

void adc_stop();

void adc_set_left_aligned();

void adc_set_right_aligned();

void adc_set_reference_voltage(enum voltage_reference v_ref);

void adc_set_trigger(uint8_t auto_trig, enum auto_trigger_source auto_trig_source);

void adc_set_prescaler(enum adc_prescalers prescaler);

void adc_enable_interrupt();

void adc_disable_interrupt();

#endif // ENV_AVR == 1 

#endif //ADC_H