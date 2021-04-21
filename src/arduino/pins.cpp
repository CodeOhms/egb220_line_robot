#ifdef ENV_ARDUINO

#include "pins.h"

uint8_t resolve_pin_num(enum pins_mcu pin_mcu)
{
    return pin_numbers_digital[(uint8_t) pin_mcu];
}

#endif //ENV_ARDUINO