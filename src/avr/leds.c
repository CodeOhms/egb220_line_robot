#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>

#include "leds.h"

void led_init()
{
    for(uint8_t l = 0; l < MOTORS_NUM; ++l)
    {
    // Setup pins as outputs:
        *(_leds.direction_regs[l]) |= (1<<_leds.pins_offset[l]);
    
    // Initialise LEDs as off:
        *(_leds.port_regs[l]) &= ~(1<<_leds.pins_offset[l]);
    }
}

void led_close()
{
    // Turn off the leds.
    for(uint8_t l = 0; l < LEDS_NUM; ++l)
    {
        *(_leds.port_regs[l]) &= ~(1<<_leds.pins_offset[l]);
    }
}

uint8_t led_exists(uint8_t led_index)
{
    if(led_index > LEDS_NUM -1)
    { // Led out of range!
        return 0;
    }
    else
    {
        return 1;
    }
}

uint8_t led_on(uint8_t led_index)
{
    if(!led_exists(led_index))
    {
        return 0;
    }
    else
    {
        *(_leds.port_regs[led_index]) |= (1<<_leds.pins_offset[led_index]);
        return 1;
    }
    return 0; // Something went wrong.
}

uint8_t led_off(uint8_t led_index)
{
    if(!led_exists(led_index))
    {
        return 0;
    }
    else
    {
        *(_leds.port_regs[led_index]) &= ~(1<<_leds.pins_offset[led_index]);
        return 1;
    }
    return 0; // Something went wrong.
}

uint8_t led_toggle(uint8_t led_index)
{
    if(!led_exists(led_index))
    {
        return 0;
    }
    else
    {
        *(_leds.port_regs[led_index]) ^= (1<<_leds.pins_offset[led_index]);
        return 1;
    }
    return 0; // Something went wrong.
}