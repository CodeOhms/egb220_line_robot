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



    ADMUX |= (1<<REFS1)|(1<<REFS0)|(1<<ADLAR); //2.56V ref, left adjusted

	// enable adc, clock prescaler of 128
	ADCSRA |= (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	ADCSRB = 0;
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
    // adc_start();
    ADCSRA |= (1<<6);
    uint16_t result = adc_read_regular();

    // Set up reading for the next sensor:
    current_sensor = (current_sensor +1) % SENSORS_NUM;
    adc_set_channel(_channels[current_sensor]);

    return result;
}