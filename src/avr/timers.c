#include "timers.h"

void timer0_select_prescaler(enum timer_prescalers prescaler)
{
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
            TCCR0A &= ~(1<<WGM02);
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