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
        case adc_prescaler2:
            ADCSRA &= ~(1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0);
            break;

        case adc_prescaler4:
            break;

        case adc_prescaler8:
            break;
        
        case adc_prescaler64:
            break;
        
        case adc_prescaler128:
            ADCSRA |= (1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0);
            break;
        
        default:
            // TODO: Something went wrong. Fatal error!
            break;
    }
}

void adc_set_channel(uint8_t channel)
{
    uint8_t mask_ADMUX = 0b00011111;

    // Clear the bits, then set them to prevent residue from old values:
    ADMUX &= ~(mask_ADMUX);

    #ifdef MUX5

        // Clear MUX5:
    ADCSRB &= ~(1<<MUX5);
    // Set MUX bits 0 to 4:
    ADMUX |= channel & mask_ADMUX;
    // Set MUX bit 5 in seperate register:
    ADCSRB |= channel & 1<<MUX5;
    /*
    The bit masking that occurs above is to ensure only the bits needed are selected.
    If the user code makes a mistake and has bits set that are outside of the 5th bit
    it will be ignored and this function will still operate as expected.
    */

    #else
    
    ADMUX = unchanged | channel;

    #endif
}

uint8_t adc_is_left_aligned()
{
    return ADMUX & ~(1<<ADLAR);
}

void adc_enable_interrupt()
{
    ADCSRA |= (1<<ADIE);
}

void adc_disable_interrupt()
{
    ADCSRA &= ~(1<<ADIE);
}

// uint8_t adc_read_left_aligned()
// {
//     return ADCH;
// }

// uint16_t adc_read_right_aligned()
// {
//     uint8_t high_bits = ADCH;
//     uint8_t low_bits = ADCL;
//     return (high_bits<<2 | low_bits);
// }

uint16_t adc_read_regular()
{
    while(ADCSRA&(1<<ADSC)){}
    uint16_t result = ADCH;
    // if(!adc_is_left_aligned())
    // {
    //     result = (ADCH<<2 | ADCL);
    // }

    return result;
}

#endif // ENV_AVR == 1 