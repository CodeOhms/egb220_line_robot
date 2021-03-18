#ifndef   LEDS_H
#define   LEDS_H

#include <stdint.h>

#include "peripherals.h"

/*
Provide a uint8_t array filled with the pin numbers
for the leds in ascending order from led0.
The array led_pins will be copied.

The pin numbers should be what is defined by
JDLuck's Arduino port.
*/
void led_init(uint8_t* led_pins);

void led_close();

uint8_t led_enable(uint8_t led_index);

uint8_t led_toggle(uint8_t led_index);

uint8_t led_builtin_enable(enum leds led_id);

uint8_t led_builtin_toggle(enum leds led_id);


#endif //LEDS_H