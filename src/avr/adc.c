#include "adc.h"

#if ENV_AVR == 1

void adc_on()
{
    ADCSRA |= (1<<ADEN);
}

void adc_off()
{
    ADCSRA &= ~(1<<ADEN);
}

void adc_start()
{
    ADCSRA |= (1<<ADSC);
}

void adc_stop()
{
    ADCSRA &= ~(1<<ADSC);
}

void adc_set_left_aligned()
{
    ADMUX |= (1<<ADLAR);
}

void adc_set_right_aligned()
{
    ADMUX &= ~(1<<ADLAR);
}

void adc_set_reference_voltage(enum voltage_reference v_ref)
{
    switch(v_ref)
    {
        case internal_off:
            ADMUX &= ~(1<<REFS1 | 1<<REFS0);
            break;
        case AVcc:
            ADMUX &= ~(1<<REFS1);
            ADMUX |= (1<<REFS0);
            break;
        case internal_on:
            ADMUX |= (1<<REFS1 | 1<<REFS0);
            break;
    }
}

void adc_set_trigger(uint8_t auto_trig, enum auto_trigger_source auto_trig_source)
{
    if(auto_trig)
    {
        ADCSRA |= (1<<ADATE);
    }

    switch(auto_trig_source)
    {
        case free_running:
            ADCSRB &= ~(1<<ADTS3 | 1<<ADTS2 | 1<<ADTS1 | 1<<ADTS0);
            break;
        // TODO: finish implementing adc_set_trigger
    }
}

void adc_set_prescaler(enum adc_prescalers prescaler)
{
    // TODO: finish implementing adc_set_prescaler
    switch(prescaler)
    {
        case prescaler2:
            ADCSRA &= ~(1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0);
            break;

        case prescaler4:
            break;

        case prescaler8:
            break;
        
        case prescaler64:
            break;
        
        case prescaler128:
            ADCSRA |= (1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0);
            break;
        
        default:
            // TODO: Something went wrong. Fatal error!
            break;
    }
}

void adc_enable_interrupt()
{
    ADCSRA |= (1<<ADIE);
}

void adc_disable_interrupt()
{
    ADCSRA &= ~(1<<ADIE);
}

#endif // ENV_AVR == 1 