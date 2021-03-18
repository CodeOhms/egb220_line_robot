#include "interrupts.h"

void interrupts_init()
{
    #if BOOTLOADER == BOOTLOADER_CATERINA
    // Stop USB triggering interrupt. Left as on by Caterina bootloader :(.
    USBCON = 0;
    #endif

    sei();
}