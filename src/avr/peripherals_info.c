#include <avr/io.h>

#include "motors.h"
#include "leds.h"

/*
This is a template file where the registers needed to run the motors must be set.
Registers for connected LEDs must be set here, too.
*/

// These defines are needed as what is provided from the AVR libraries are not constants.
// #define _PORTB (volatile uint8_t*) 0x05 + __SFR_OFFSET
// #define _PORTC (volatile uint8_t*) 0x08 + __SFR_OFFSET
// #define _PORTD (volatile uint8_t*) 0x0B + __SFR_OFFSET
// #define _PORTE (volatile uint8_t*) 0x0E + __SFR_OFFSET
// #define _DDRB (volatile uint8_t*) 0x04 + __SFR_OFFSET
// #define _DDRC (volatile uint8_t*) 0x07 + __SFR_OFFSET
// #define _DDRD (volatile uint8_t*) 0x0A + __SFR_OFFSET
// #define _DDRE (volatile uint8_t*) 0x0D + __SFR_OFFSET
// #define _OCR0A (volatile uint8_t*) 0x27 + __SFR_OFFSET
// #define _OCR0B (volatile uint8_t*) 0x28 + __SFR_OFFSET


#define TEMP_SFR_IO8 _SFR_IO8
#undef _SFR_IO8
#define _SFR_IO8(io_addr) ((volatile uint8_t*) (io_addr + __SFR_OFFSET))

const motors_info _motors =
{
    .port_regs = {
        // Phase pins:
        PORTB, // Motor A
        PORTE, // Motor B
    },
    .direction_regs = {
        // Motor A:
        {
            DDRB, // Phase
            DDRB  // Enable
        },
        // Motor B:
        {
            DDRE, // Phase
            DDRD  // Enable
        }
    },
    .compare_regs = {
        OCR0A, // Motor A
        OCR0B, // Motor B
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
        PORTC,
        PORTB,
        PORTB,
    },
    .direction_regs = {
        DDRC,
        DDRB,
        DDRB,
    },
    .pins_offset = {
        PORTC6,
        PORTB5,
        PORTB6,
    }
};

const uint8_t _channels[SENSORS_NUM] =
{
    // 0b000100, // ADC4
    // 0b000101, // ADC5
    // 0b000110, // ADC6
    // 0b000111, // ADC7
    // 0b100011, // ADC11
    // 0b100010, // ADC10
    // 0b100001, // ADC9
    // 0b100000, // ADC8

    0b000000, // ADC0
    0b000001, // ADC1
    0b000100, // ADC4
    0b000101, // ADC5
    0b100000, // ADC8
    0b100001, // ADC9
    0b100011, // ADC11
    0b100010, // ADC10
};

#undef _SFR_IO8
#define _SFR_IO8 TEMP_SFR_IO8
#undef TEMP_SFR_IO8