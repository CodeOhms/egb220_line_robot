#include "include/leds.h"


uint8_t num_pins = 0;
uint8_t* _led_pins = 0;

void led_init(uint8_t* led_pins)
{
    // Allocate dynamic memory for led_pins 'array'.
    uint8_t num_pins = sizeof(led_pins)/sizeof(uint8_t);
    _led_pins = malloc(sizeof(led_pins));

    // Copy the array into led_pins.
    for(uint8_t i = 0; i < num_pins; ++i)
    {
        _led_pins[i] = led_pins[i];
    }
}

void led_close()
{
    // Free allocated memory.
    free(_led_pins);
    num_pins = 0;

    // Reset the PORT and PIN registers to defaults.
}

int led_exists(uint8_t led_index)
{
    if(led_index > num_pins -1)
    { // Led out of range!
        return 0;
    }
    else
    {
        return 1;
    }
}

int led_enable(uint8_t led_index)
{
    if(!led_exists())
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

int led_toggle(uint8_t led_index)
{
    if(!led_exists())
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

// void led_builtin_enable(enum leds led_id)
// {
//     pinMode(_led_pins[led_id], OUTPUT);
// }

// void led_builtin_toggle(enum leds led_id)
// {
//     digitalWrite(_led_pins[led_id], !digitalRead(_led_pins[led_id]));
// }

void led_builtin_enable(enum leds led_id)
{
    led_enable((uint8_t) led_id);
}

void led_builtin_toggle(enum leds led_id)
{
    led_toggle((uint8_t) led_id);
}