#if ENV_AVR == 1

#include "pins.h"

void pins_init()
{
    port_registers[0] = PORTB;
    port_registers[1] = PORTC;
    port_registers[2] = PORTD;
    port_registers[3] = PORTE;
    port_registers[4] = PORTF;

    direction_registers[0] = DDRB;
    direction_registers[1] = DDRC;
    direction_registers[2] = DDRD;
    direction_registers[3] = DDRE;
    direction_registers[4] = DDRF;
}

uint8_t* resolve_port_reg(enum pins_mcu pin_mcu)
{
    // Truncating here is deliberate!
    // E.G. PC_pin1 = 9/8 = 1.25 --> port index is 1.
    uint8_t port = ((uint8_t) pin_mcu) / 8;
    return port_registers[port];
}

uint8_t* resolve_direction_reg(enum pins_mcu pin_mcu)
{
    // Truncating here is deliberate!
    // E.G. PC_pin1 = 9/8 = 1.25 --> port index is 1.
    uint8_t port = ((uint8_t) pin_mcu) / 8;
    return direction_registers[port];
}

uint8_t resolve_pin_offset(enum pins_mcu pin_mcu)
{
    return pin_mcu % 8;
}

#endif //ENV_AVR

#if ENV_ARDUINO == 1

uint8_t resolve_pin_num(enum pins_mcu pin_mcu)
{
    return pin_numbers_digital[(uint8_t) pin_mcu];
}

#endif //ENV_ARDUINO