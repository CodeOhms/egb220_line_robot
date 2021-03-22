#ifdef ENV_AVR

#include "pins.h"

pin_extra_info* pins_extra_info[40] = {
    // Port B:
    0,
    0,
    0,
    0,
    0,
    &info_PB_Pin5,
    &info_PB_Pin6,
    &info_PB_Pin7,
    // Port C:
    0, // Doesn't exist.
    0, // Doesn't exist.
    0, // Doesn't exist.
    0, // Doesn't exist.
    0, // Doesn't exist.
    0, // Doesn't exist.
    0,
    0,
    // Port D:
    &info_PD_Pin0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    // Port E:
    0, // Doesn't exist.
    0, // Doesn't exist.
    0,
    0, // Doesn't exist.
    0, // Doesn't exist.
    0, // Doesn't exist.
    0,
    0, // Doesn't exist.
    // Port F:
    0,
    0,
    0, // Doesn't exist.
    0, // Doesn't exist.
    0,
    0,
    0,
    0,
};

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
    compare_registers[2] = &OCR1A;
    compare_registers[3] = &OCR1B;

    info_PB_Pin7.compare_reg_index = 0;
    info_PB_Pin7.pwm_inverting_mode_bits_offsets = (1<<COM0A1) | (1<<COM0A0);
    info_PD_Pin0.compare_reg_index = 1;
    info_PD_Pin0.pwm_inverting_mode_bits_offsets = (1<<COM0B1) | (1<<COM0B0);
}

uint8_t resolve_pwm_inverting_mode_bits_offset_container(enum pins_mcu pin_mcu)
{
    return (pins_extra_info[(uint8_t) pin_mcu])->pwm_inverting_mode_bits_offsets;
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

    uint8_t offset_current = 0;
    // Loop through all bits:
    for(uint8_t bit_current = 0; bit_current < 8; ++bit_current)
    {    // Check current bit is 1:
        if( (1<<bit_current & bits_offsets_container) >> bit_current )
        {
            inverting_bits_offsets[offset_current] = bit_current;
            ++offset_current;
        }
    }

    return 1;
}

volatile uint8_t* resolve_port_reg(enum pins_mcu pin_mcu)
{
    // Truncating here is deliberate!
    // E.G. PC_pin1 = 9/8 = 1.25 --> port index is 1.
    uint8_t port = ((uint8_t) pin_mcu) / 8;
    return port_registers[port];
}

volatile uint8_t* resolve_direction_reg(enum pins_mcu pin_mcu)
{
    // Truncating here is deliberate!
    // E.G. PC_pin1 = 9/8 = 1.25 --> port index is 1.
    uint8_t port = ((uint8_t) pin_mcu) / 8;
    return direction_registers[port];
}

uint8_t exists_compare_reg(enum pins_mcu pin_mcu)
{
    // If there is not a null ptr at this index, it might exist:
    pin_extra_info* extra_info = pins_extra_info[(uint8_t) pin_mcu];
    if(extra_info)
    {
        if(extra_info->compare_reg_index <= (sizeof(compare_registers)/sizeof(compare_registers[0])) -1)
        {
            return 1;
        }
    }

    return 0;
}

volatile uint8_t* resolve_compare_reg(enum pins_mcu pin_mcu)
{
    uint8_t index = (pins_extra_info[(uint8_t) pin_mcu])->compare_reg_index;
    return compare_registers[index];
}

uint8_t resolve_pin_offset(enum pins_mcu pin_mcu)
{
    return ((uint8_t) pin_mcu) % 8;
}

#endif //ENV_AVR