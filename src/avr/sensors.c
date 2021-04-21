#include "sensors.h"

void sensors_init()
{
    // Set-up ADC:
        // Voltage reference:
    adc_set_reference_voltage(AVcc);
        // Left adjusted readings:
    adc_set_left_aligned();
        // Trigger, auto trigger enabled and free running mode:
    adc_set_trigger(1, free_running);
        // Prescaler:
    adc_set_prescaler(prescaler128);
    
    // Turn on ADC:
    adc_on();

    // Start ADC conversion/reading:
    adc_start();
}