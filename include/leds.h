#ifndef   LEDS_H
#define   LEDS_H

#include <stdint.h>

#include "peripherals.h"

typedef struct leds_info_t
{
    volatile uint8_t* port_regs[LEDS_NUM];
    volatile uint8_t* direction_regs[LEDS_NUM];
    uint8_t pins_offset[LEDS_NUM]; // E.G. offset 6 with port E is PE6.
} leds_info;

extern const leds_info _leds;

/*
Provide a uint8_t array filled with the pin numbers
for the leds in ascending order from led0.
The array led_pins will be copied.

The pin numbers should be what is defined by
JDLuck's Arduino port.
*/
void led_init();

void led_close();

uint8_t led_on(uint8_t led_index);

uint8_t led_off(uint8_t led_index);

uint8_t led_toggle(uint8_t led_index);

#endif //LEDS_H