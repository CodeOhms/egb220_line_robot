#include "sensors.h"

void sensor_init()
{
    // Set-up ADC:
        // Voltage reference:
    // adc_set_reference_voltage(AVcc);
    //     // Left adjusted readings:
    // adc_set_left_aligned();
    //     // Trigger, auto trigger enabled and free running mode:
    // adc_set_trigger(1, free_running);
    //     // Prescaler:
    // adc_set_prescaler(adc_prescaler128);
    
    // // Set up reading for the first sensor:
    // adc_set_channel(_channels[0]);

    // // // Turn on ADC:
    // adc_on();

    // // // Start ADC conversion/reading:
    // adc_start();



    ADMUX |= (1<<6)|(1<<5); //5V ref 
	ADCSRA |= (1<<7)|(1<<5)|(1<<4)|(1<<3)|(1<<2)|(1<<1);
	// enable adc, enable auto-trigger, enable interrupt & interrupt flag, clock prescaler of 128, start converion, left adjusted
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
    uint16_t result = adc_read_regular();

    // Set up reading for the next sensor:
    current_sensor = (current_sensor +1) % SENSORS_NUM;
    adc_set_channel(_channels[current_sensor]);

    return result;
}