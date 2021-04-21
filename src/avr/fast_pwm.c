#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>

#include "fast_pwm.h"

uint8_t _fast_pwm_is_initialised = 0;

void pwm_inverted()
{
    // Motor A:
    TCCR0A |= (1<<COM0A1);
	TCCR0A |= (1<<COM0A0);

    // Motor B:
    TCCR0A |= (1<<COM0B1);
	TCCR0A |= (1<<COM0B0);
}

void pwm_non_inverted()
{
    // Motor A:
    TCCR0A |= (1<<COM0A1);
	TCCR0A &= ~(1<<COM0A0);

    // Motor B:
    TCCR0A |= (1<<COM0B1);
	TCCR0A &= ~(1<<COM0B0);
}

uint8_t fast_pwm_init(enum pwm_prescalers prescaler, uint8_t pwm_is_inverted)
{
    // Set up fast pwm with default TOP of 0xFF (255):
    TCCR0A |= (1<<WGM00);
    TCCR0A |= (1<<WGM01);
    // TCCR0A &= ~(1<<WGM02);

    // Select prescaler:
    fast_pwm_select_prescaler(prescaler);    

    // Set pwm inverting mode:
    if(pwm_is_inverted)
    {
        pwm_inverted();
    }
    else
    {
        pwm_non_inverted();
    }

    // User code must set PWM pins as outputs!

    _fast_pwm_is_initialised = 1;

    return 1;
}

void fast_pwm_close()
{
    // Select no prescaler:
    fast_pwm_select_prescaler(no_prescaler);
    
    // Disable fast pwm mode (set all to zero):
    TCCR0A &= ~(1<<WGM00);
    TCCR0A &= ~(1<<WGM01);
    TCCR0A &= ~(1<<WGM02);

    _fast_pwm_is_initialised = 0;
}

void fast_pwm_select_prescaler(enum pwm_prescalers prescaler)
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
    
    case prescaler8:
        TCCR0B &= ~(1<<CS02);
        TCCR0B |= (1<<CS01);
        TCCR0B &= ~(1<<CS00);
        break;
    
    case prescaler64:
        TCCR0B &= ~(1<<CS02);
        TCCR0B |= (1<<CS01);
        TCCR0B |= (1<<CS00);
        break;
    
    case prescaler256:
        TCCR0B |= (1<<CS02);
        TCCR0B &= ~(1<<CS01);
        TCCR0B &= ~(1<<CS00);
        break;
    
    case prescaler1024:
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