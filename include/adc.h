#ifndef  ADC_H
#define  ADC_H

#if ENV_AVR == 1

#include <avr/io.h>

#include "peripherals.h"

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

/*
Used to set the MUX bits for analog channel selection.
Make use of AVR definitions, `in avr/io.h`, to set the channel.

Returns:
0 if the given channel is out of range. I.e. if channel is 6 and MUX_BITS_NUM is 6 then
`6 < 6 -1` is false.
*/
uint8_t adc_set_channel(uint8_t channel);

void adc_enable_interrupt();

void adc_disable_interrupt();

uint8_t adc_read_left_aligned();

uint16_t adc_read_right_aligned();

#endif // ENV_AVR == 1 

#endif //ADC_H