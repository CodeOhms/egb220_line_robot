#include "sensors.h"

void sensor_init()
{
    // Set-up ADC:
        // Voltage reference:
    adc_set_reference_voltage(AVcc); // 5v refs
        // Left adjusted readings:
    adc_set_left_aligned();
        // Prescaler:
    adc_set_prescaler(adc_prescaler128);
        // Trigger, auto trigger disabled and free running mode:
    adc_set_trigger(0, free_running);
    // Set up reading for the first sensor:
    adc_set_channel(_channels[0]);

    // Turn on ADC:
    adc_on();
}

void sensor_close()
{
    // Turn off ADC:
    adc_stop();
    adc_off();
}

uint16_t sensor_read(uint8_t current_sensor)
{   
    // Read the sensor:
    adc_start();
    uint16_t result = adc_read_regular();

    // Set up reading for the next sensor:
    current_sensor = (current_sensor +1) % SENSORS_NUM;
    adc_set_channel(_channels[current_sensor]);

    return result;
}