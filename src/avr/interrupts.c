#include <avr/interrupt.h>
#include "interrupts.h"

void interrupts_init(void)
{
    #if BOOTLOADER == 0
    // Stop USB triggering interrupt. Left as on by Caterina bootloader :(.
    USBCON = 0;
    #endif

    sei();
}