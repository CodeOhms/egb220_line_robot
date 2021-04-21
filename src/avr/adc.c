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

uint8_t adc_set_channel(uint8_t channel)
{
    if(channel > MUX_BITS_NUM -1)
    {
        return 0;
    }
    
    uint8_t unchanged = ADMUX & 0b11100000;
    #if MUX_BITS_NUM > 5

    // Set MUX bits 0 to 4:
    ADMUX = unchanged | (channel & 0b00011111);
    // Set MUX bit 5 in seperate register:
    uint8_t bit5 = (channel & 0b00100000)>>MUX5;
    ADCSRB |= (bit5<<MUX5);

    #else
    
    ADMUX = unchanged | channel;

    #endif
    return 1;
}

void adc_enable_interrupt()
{
    ADCSRA |= (1<<ADIE);
}

void adc_disable_interrupt()
{
    ADCSRA &= ~(1<<ADIE);
}

uint8_t adc_read_left_aligned()
{
    return ADCH;
}

uint16_t adc_read_right_aligned()
{
    uint8_t high_bits = ADCH;
    uint8_t low_bits = ADCL;
    return (high_bits<<2 | low_bits);
}

#endif // ENV_AVR == 1 