#include <avr/io.h>

#include "motors.h"
#include "leds.h"

/*
This is a template file where the registers needed to run the motors must be set.
Registers for connected LEDs must be set here, too.
*/

// These defines are needed as what is provided from the AVR libraries are not constants.
#define _PORTB (volatile uint8_t*) 0x05 + __SFR_OFFSET
#define _PORTC (volatile uint8_t*) 0x08 + __SFR_OFFSET
#define _PORTD (volatile uint8_t*) 0x0B + __SFR_OFFSET
#define _PORTE (volatile uint8_t*) 0x0E + __SFR_OFFSET
#define _DDRB (volatile uint8_t*) 0x04 + __SFR_OFFSET
#define _DDRC (volatile uint8_t*) 0x07 + __SFR_OFFSET
#define _DDRD (volatile uint8_t*) 0x0A + __SFR_OFFSET
#define _DDRE (volatile uint8_t*) 0x0D + __SFR_OFFSET
#define _OCR0A (volatile uint8_t*) 0x27 + __SFR_OFFSET
#define _OCR0B (volatile uint8_t*) 0x28 + __SFR_OFFSET

const motors_info _motors =
{
    .port_regs = {
        // Phase pins:
        _PORTB, // Motor A
        _PORTE, // Motor B
    },
    .direction_regs = {
        // Motor A:
        {
            _DDRB, // Phase
            _DDRB  // Enable
        },
        // Motor B:
        {
            _DDRE, // Phase
            _DDRD  // Enable
        }
    },
    .compare_regs = {
        _OCR0A, // Motor A
        _OCR0B, // Motor B
    },
    .pins_offset = {
        // Motor A:
        {
            PORTB0, // Phase
            PORTB7  // Enable
        },
        // Motor B:
        {
            PORTE6, // Phase
            PORTD0  // Enable
        }
    }
};

const leds_info _leds =
{
    .port_regs = {
        _PORTC,
        _PORTB,
        _PORTB,
    },
    .direction_regs = {
        _DDRC,
        _DDRB,
        _DDRB,
    },
    .pins_offset = {
        PORTC6,
        PORTB5,
        PORTB6,
    }
};