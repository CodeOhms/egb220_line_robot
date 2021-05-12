#include "timers.h"

enum timer_prescalers _timer_prescaler_selected;

void timer0_set_prescaler(enum timer_prescalers prescaler)
{
    _timer_prescaler_selected = prescaler;
    switch(prescaler)
    {
    case no_clock_source:
        TCCR0B &= ~(1<<CS02);
        TCCR0B &= ~(1<<CS01);
        TCCR0B &= ~(1<<CS00);
        break;
    
    case no_prescaler:
        TCCR0B &= ~(1<<CS02);
        TCCR0B &= ~(1<<CS01);
        TCCR0B |= (1<<CS00);
        break;
    
    case timer_prescaler8:
        TCCR0B &= ~(1<<CS02);
        TCCR0B |= (1<<CS01);
        TCCR0B &= ~(1<<CS00);
        break;
    
    case timer_prescaler64:
        TCCR0B &= ~(1<<CS02);
        TCCR0B |= (1<<CS01);
        TCCR0B |= (1<<CS00);
        break;
    
    case timer_prescaler256:
        TCCR0B |= (1<<CS02);
        TCCR0B &= ~(1<<CS01);
        TCCR0B &= ~(1<<CS00);
        break;
    
    case timer_prescaler1024:
        TCCR0B |= (1<<CS02);
        TCCR0B &= ~(1<<CS01);
        TCCR0B |= (1<<CS00);
        break;
    
    case extern_clock_falling:
        TCCR0B |= (1<<CS02);
        TCCR0B |= (1<<CS01);
        TCCR0B &= ~(1<<CS00);
        break;
    
    case extern_clock_rising:
        TCCR0B |= (1<<CS02);
        TCCR0B |= (1<<CS01);
        TCCR0B |= (1<<CS00);
        break;
    
    default:
        // TODO: Something went wrong. Fatal error!
        break;
    }
}

enum timer_prescalers timer0_get_prescaler()
{
    return _timer_prescaler_selected;
}

uint16_t timer_prescaler_enum_to_int(enum timer_prescalers prescaler)
{
    uint16_t prescaler_int = 0;
    switch(prescaler)
    {
        case timer_prescaler8:
            prescaler_int = 8;
            break;
        
        case timer_prescaler64:
            prescaler_int = 64;
            break;
        
        case timer_prescaler256:
            prescaler_int = 256;
            break;
        
        case timer_prescaler1024:
            prescaler_int = 1024;
            break;
        
        default:
            // TODO: Something went wrong. Fatal error!
            break;
    }

    return prescaler_int;
}

void timer0_waveform_generation_mode(enum waveform_generation_mode wgm)
{
    switch(wgm)
    {
        case normal:
            break;
        case pwm_phase_correct_0xFF:
            break;
        case CTC:
            break;
        case fast_pwm_0xFF:
            TCCR0A |= (1<<WGM00);
            TCCR0A |= (1<<WGM01);
            TCCR0B &= ~(1<<WGM02);
            break;
        case pwm_phase_correct_OCRx:
            break;
        case fast_pwm_OCRx:
            break;

        default:
            // TODO: Something went wrong. Fatal error!
            break;
    }
}

void timer0_compare_output_mode(enum compare_output_mode cop, enum compare_output comp_out)
{
    uint8_t com0x0 = 0; // Bit offset of compare output bit 0, e.g. COM0A0 = 6
    switch (comp_out)
    {
        case comp_A:
            com0x0 = 6;
            break;
        case comp_B:
            com0x0 = 4;
            break;
        case comp_C:
            // Not defined for 8 bit timers! ERROR!!
            //com0x0 = 2;
            break;
    
        default:
            break;
    }

    switch(cop)
    {
        case com_normal:
            TCCR0A &= ~(1<<com0x0);
            TCCR0A &= ~(1<<(com0x0 + 1));
            break;

        case com_toggle:
            TCCR0A |= (1<<com0x0);
            TCCR0A &= ~(1<<(com0x0 + 1));
            break;

        case com_clear:
            TCCR0A &= ~(1<<com0x0);
            TCCR0A |= (1<<(com0x0 + 1));
            break;

        case com_set:
            TCCR0A |= (1<<com0x0);
            TCCR0A |= (1<<(com0x0 + 1));
            break;

        default:
            break;
            // TODO: Something went wrong. Fatal error!
    }
}