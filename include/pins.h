#ifndef  PINS_H
#define  PINS_H

#include <stdint.h>

enum pins_mcu
{
    // Port B:
    PB_Pin0,
    PB_Pin1,
    PB_Pin2,
    PB_Pin3,
    PB_Pin4,
    PB_Pin5,
    PB_Pin6,
    PB_Pin7,
    // Port C:
    PC_Pin0,
    PC_Pin1,
    PC_Pin2,
    PC_Pin3,
    PC_Pin4,
    PC_Pin5,
    PC_Pin6,
    PC_Pin7,
    // Port D:
    PD_Pin0,
    PD_Pin1,
    PD_Pin2,
    PD_Pin3,
    PD_Pin4,
    PD_Pin5,
    PD_Pin6,
    PD_Pin7,
    // Port E:
    PE_Pin0,
    PE_Pin1,
    PE_Pin2,
    PE_Pin3,
    PE_Pin4,
    PE_Pin5,
    PE_Pin6,
    PE_Pin7,
    // Port F:
    PF_Pin0,
    PF_Pin1,
    PF_Pin2,
    PF_Pin3,
    PF_Pin4,
    PF_Pin5,
    PF_Pin6,
    PF_Pin7,
};

#if ENV_AVR == 1
#include <avr/io.h>

uint8_t* port_registers[5];
uint8_t* direction_registers[5];

/*
Translates enums from `pins_mcu` to the corresponding port register.
*/
uint8_t* resolve_port_reg(enum pins_mcu pin_mcu);

/*
Translates enums from `pins_mcu` to the corresponding direction register.
*/
uint8_t* resolve_direction_reg(enum pins_mcu pin_mcu);

/*
Translates enums from `pins_mcu` to the corresponding pin offset/number of its port.
// E.G. offset 6 with port E = PE_Pin6.
*/
uint8_t resolve_pin_offset(enum pins_mcu pin_mcu);

#endif //ENV_AVR


#if ENV_ARDUINO == 1
uint8_t pin_numbers[]
{
    // TODO: fill this out with the pin numbers!!!
};

uint8_t resolve_pin_num(enum pins_mcu pin_mcu);
#endif //ENV_ARDUINO

/*
Does nothing if ENV_ARDUINO == 1.
If ENV_AVR == 1,
sets up the `port_registers` and `direction_registers` arrays.
*/
void pins_mcu_init();

#endif //PINS_H