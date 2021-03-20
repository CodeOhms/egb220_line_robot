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
    PC_Pin0, // Doesn't exist.
    PC_Pin1, // Doesn't exist.
    PC_Pin2, // Doesn't exist.
    PC_Pin3, // Doesn't exist.
    PC_Pin4, // Doesn't exist.
    PC_Pin5, // Doesn't exist.
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
    PE_Pin0, // Doesn't exist.
    PE_Pin1, // Doesn't exist.
    PE_Pin2,
    PE_Pin3, // Doesn't exist.
    PE_Pin4, // Doesn't exist.
    PE_Pin5, // Doesn't exist.
    PE_Pin6,
    PE_Pin7, // Doesn't exist.
    // Port F:
    PF_Pin0,
    PF_Pin1,
    PF_Pin2, // Doesn't exist.
    PF_Pin3, // Doesn't exist.
    PF_Pin4,
    PF_Pin5,
    PF_Pin6,
    PF_Pin7,
};

#ifdef ENV_AVR
#include <avr/io.h>

volatile uint8_t* port_registers[5];
volatile uint8_t* direction_registers[5];

/*
Translates enums from `pins_mcu` to the corresponding port register.
*/
volatile uint8_t* resolve_port_reg(enum pins_mcu pin_mcu);

/*
Translates enums from `pins_mcu` to the corresponding direction register.
*/
volatile uint8_t* resolve_direction_reg(enum pins_mcu pin_mcu);

/*
Translates enums from `pins_mcu` to the corresponding pin offset/number of its port.
// E.G. offset 6 with port E = PE_Pin6.
*/
uint8_t resolve_pin_offset(enum pins_mcu pin_mcu);

#endif //ENV_AVR


#ifdef ENV_ARDUINO
uint8_t pin_numbers_digital[]
{
    // TODO: fill this out with the Arduino pin numbers!!!
    // Port B
    17,
    15,
    16,
    14,
    8,
    9,
    10,
    11,
    // Port C
    0,
    0,
    0,
    0,
    0,
    0,
    5,
    13,
    // Port D
    3,
    2,
    0,
    1,
    4,
    30,
    12,
    6,
    // Port E
    0,
    0,
    HWB,
    0,
    0,
    0,
    7,
    0,
    // Port F
    23,
    22,
    0,
    0,
    21,
    20,
    19,
    18
};

uint8_t resolve_pin_num(enum pins_mcu pin_mcu);
#endif //ENV_ARDUINO

/*
Does nothing if ENV_ARDUINO == 1.
If ENV_AVR == 1,
sets up the `port_registers` and `direction_registers` arrays.
*/
void pins_init();

#endif //PINS_H