#include "sensors.h"

uint8_t _current_sensor;

void sensor_init()
{
    // Set-up ADC:
        // Voltage reference:
    adc_set_reference_voltage(AVcc);
        // Left adjusted readings:
    adc_set_left_aligned();
        // Trigger, auto trigger enabled and free running mode:
    adc_set_trigger(1, free_running);
        // Prescaler:
    adc_set_prescaler(adc_prescaler128);
    
    // Set up reading for the first sensor:
    _current_sensor = 0;
    adc_set_channel(_channels[_current_sensor]);

    // Turn on ADC:
    adc_on();

    // Start ADC conversion/reading:
    adc_start();
}

void sensor_close()
{
    // Turn off ADC:
    adc_stop();
    adc_off();
}

uint16_t sensor_read()
{   
    // Read the sensor:
    uint16_t result = adc_read_regular();

    // Set up reading for the next sensor:
    _current_sensor += 1;
    adc_set_channel(_channels[_current_sensor]);

    return result;
}