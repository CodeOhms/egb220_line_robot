#include "motors.h"

/*
This is a template file where the registers needed to run the motors must be set.
*/

// These defines are needed as what is provided from the AVR libraries are not constants.
#define PB (volatile uint8_t*) 0x25
#define PD (volatile uint8_t*) 0x2B
#define PE (volatile uint8_t*) 0x2E
#define DB (volatile uint8_t*) 0x24
#define DD (volatile uint8_t*) 0x2A
#define DE (volatile uint8_t*) 0x2D
#define OC0A (volatile uint8_t*) 0x47
#define OC0B (volatile uint8_t*) 0x48

motors_info _motors =
{
    .port_regs = {
        // Phase pins:
        PB, // Motor A
        PE, // Motor B
    },
    .direction_regs = {
        // Motor A:
        {
            DB, // Phase
            DB  // Enable
        },
        // Motor B:
        {
            DE, // Phase
            DD  // Enable
        }
    },
    .compare_regs = {
        OC0A, // Motor A
        OC0B, // Motor B
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