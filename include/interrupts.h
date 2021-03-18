#ifndef  INTERRUPTS_H
#define  INTERRUPTS_H

#include <avr/interrupt.h>

#define BOOTLOADER_CATERINA 0
#define BOOTLOADER_DFU 1

void interrupts_init();

#endif //INTERRUPTS_H