#ifdef ENV_AVR

#include "pins.h"

void pins_init()
{
    port_registers[0] = &PORTB;
    port_registers[1] = &PORTC;
    port_registers[2] = &PORTD;
    port_registers[3] = &PORTE;
    port_registers[4] = &PORTF;

    direction_registers[0] = &DDRB;
    direction_registers[1] = &DDRC;
    direction_registers[2] = &DDRD;
    direction_registers[3] = &DDRE;
    direction_registers[4] = &DDRF;

    compare_registers[0] = &OCR0A;
    compare_registers[1] = &OCR0B;

    pwm_inverting_mode_bits[0] = (1<<COM0A1) | (1<<COM0A0);
    pwm_inverting_mode_bits[1] = (1<<COM0B1) | (1<<COM0B0);
}

uint8_t resolve_pwm_inverting_mode_bits_offset_container(enum pins_mcu pin_mcu)
{
    // Find the index of the corresponding compare register.
    uint8_t index = read_compare_reg_index(pin_mcu);
    return pwm_inverting_mode_bits[index];
}

uint8_t read_pwm_inverting_mode_bits_offset(enum pins_mcu pin_mcu, uint8_t* inverting_bits_offsets)
{
    uint8_t bits_offsets_container = resolve_pwm_inverting_mode_bits_offset_container(pin_mcu);
    
    // Search for the 0th bit first.
    uint8_t num_offsets = sizeof(inverting_bits_offsets)/sizeof(inverting_bits_offsets[0]);
    if(num_offsets == 0)
    {
        return 0;
    }
    for(uint8_t offset_current = 0; offset_current < num_offsets; ++offset_current)
    {
        // Loop through all bits:
        for(uint8_t bit_current = 0; bit_current < sizeof(bits_offsets_container); ++ bit_current)
        {    // Check current bit is 1:
            if( (1<<bit_current & bits_offsets_container) >> bit_current )
            {
                inverting_bits_offsets[offset_current] = 1<<bit_current;
            }
        }
    }

    return 1;
}

uint8_t read_enum_index(enum pins_mcu pin_mcu)
{
    return ((uint8_t) pin_mcu) & 0xFF;
}

uint8_t read_compare_reg_index(enum pins_mcu pin_mcu)
{
    return ((uint8_t) pin_mcu >> 8) & 0xFF; 
}

volatile uint8_t* resolve_port_reg(enum pins_mcu pin_mcu)
{
    // Truncating here is deliberate!
    // E.G. PC_pin1 = 9/8 = 1.25 --> port index is 1.
    uint8_t index = read_enum_index(pin_mcu);
    uint8_t port = index / 8;
    return port_registers[port];
}

volatile uint8_t* resolve_direction_reg(enum pins_mcu pin_mcu)
{
    // Truncating here is deliberate!
    // E.G. PC_pin1 = 9/8 = 1.25 --> port index is 1.
    uint8_t index = read_enum_index(pin_mcu);
    uint8_t port = index / 8;
    return direction_registers[port];
}

uint8_t exists_compare_reg(enum pins_mcu pin_mcu)
{
    if( ((uint8_t) pin_mcu >> 15) )
    {
        return 0;
    }
    return 1;
}

volatile uint8_t* resolve_compare_reg(enum pins_mcu pin_mcu)
{
    uint8_t index = read_compare_reg_index(pin_mcu);
    return compare_registers[index];
}

uint8_t resolve_pin_offset(enum pins_mcu pin_mcu)
{
    uint8_t index = read_enum_index(pin_mcu);
    return index % 8;
}

#endif //ENV_AVR