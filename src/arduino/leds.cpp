#ifdef ENV_ARDUINO

#include <stdlib.h>
#include <Arduino.h>

#include "leds.h"

uint8_t _num_pins = 0;
uint8_t* _led_pins = 0;

void led_init(enum pins_mcu* led_pins)
{
    // Allocate dynamic memory for led_pins 'array'.
    _num_pins = sizeof(led_pins)/sizeof(uint8_t);
    _led_pins = (uint8_t*) malloc(sizeof(led_pins));

    // Copy the array into led_pins.
    for(uint8_t i = 0; i < _num_pins; ++i)
    {
        _led_pins[i] = led_pins[i];
    }

    // Setup led pins for output mode.
    for(uint8_t i = 0; i < _num_pins; ++i)
    {
        pinMode(_led_pins[i], OUTPUT);
    }
}

void led_close()
{
    // Turn off the leds.
    for(uint8_t i = 0; i < _num_pins; ++i)
    {
        digitalWrite(_led_pins[i], LOW);
    }

    // Free allocated memory.
    free(_led_pins);
    _num_pins = 0;
}

uint8_t led_exists(uint8_t led_index)
{
    if(led_index > _num_pins -1)
    { // Led out of range!
        return 0;
    }
    else
    {
        return 1;
    }
}

uint8_t led_enable(uint8_t led_index)
{
    if(!led_exists(led_index))
    {
        return 0;
    }
    else
    {
        pinMode(_led_pins[led_index], OUTPUT);
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
        digitalWrite(_led_pins[led_index], !digitalRead(_led_pins[led_index]));
        return 1;
    }
    return 0; // Something went wrong.
}

uint8_t led_builtin_enable(enum leds led_id)
{
    return led_enable((uint8_t) led_id);
}

uint8_t led_builtin_toggle(enum leds led_id)
{
    return led_toggle((uint8_t) led_id);
}

#endif //ENV_ARDUINO