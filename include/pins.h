#ifndef  PINS_H
#define  PINS_H

#include <stdint.h>

/*
0st byte: index in enum pins. 1nd byte: index in register array.
Last bit HIGH (on the left end) indicates if it has an associated pwm compare pin.
*/
enum pins_mcu
{
    // Port B:
    PB_Pin0 = (uint16_t) (1 << 15) | 0U,
    PB_Pin1 = (uint16_t) (1 << 15) | 1U,
    PB_Pin2 = (uint16_t) (1 << 15) | 2U,
    PB_Pin3 = (uint16_t) (1 << 15) | 3U,
    PB_Pin4 = (uint16_t) (1 << 15) | 4U,
    PB_Pin5 = (uint16_t) (1 << 15) | 5U,
    PB_Pin6 = (uint16_t) (1 << 15) | 6U,
    PB_Pin7 = (uint16_t) (0 << 8) | 7U,
    // Port C16
    PC_Pin0 = (uint16_t) (1 << 15) | 8U, // Doesn't exist.
    PC_Pin1 = (uint16_t) (1 << 15) | 9U, // Doesn't exist.
    PC_Pin2 = (uint16_t) (1 << 15) | 10U, // Doesn't exist.
    PC_Pin3 = (uint16_t) (1 << 15) | 11U, // Doesn't exist.
    PC_Pin4 = (uint16_t) (1 << 15) | 12U, // Doesn't exist.
    PC_Pin5 = (uint16_t) (1 << 15) | 13U, // Doesn't exist.
    PC_Pin6 = (uint16_t) (1 << 15) | 14U,
    PC_Pin7 = (uint16_t) (1 << 15) | 15U,
    // Port D:
    PD_Pin0 = (uint16_t) (1 << 8) | 16U,
    PD_Pin1 = (uint16_t) (1 << 15) | 17U,
    PD_Pin2 = (uint16_t) (1 << 15) | 18U,
    PD_Pin3 = (uint16_t) (1 << 15) | 19U,
    PD_Pin4 = (uint16_t) (1 << 15) | 20U,
    PD_Pin5 = (uint16_t) (1 << 15) | 21U,
    PD_Pin6 = (uint16_t) (1 << 15) | 22U,
    PD_Pin7 = (uint16_t) (1 << 15) | 23U,
    // Port E:
    PE_Pin0 = (uint16_t) (1 << 15) | 24U, // Doesn't exist.
    PE_Pin1 = (uint16_t) (1 << 15) | 25U, // Doesn't exist.
    PE_Pin2 = (uint16_t) (1 << 15) | 26U,
    PE_Pin3 = (uint16_t) (1 << 15) | 27U, // Doesn't exist.
    PE_Pin4 = (uint16_t) (1 << 15) | 28U, // Doesn't exist.
    PE_Pin5 = (uint16_t) (1 << 15) | 29U, // Doesn't exist.
    PE_Pin6 = (uint16_t) (1 << 15) | 30U,
    PE_Pin7 = (uint16_t) (1 << 15) | 31U, // Doesn't exist.
    // Port F16
    PF_Pin0 = (uint16_t) (1 << 15) | 32U,
    PF_Pin1 = (uint16_t) (1 << 15) | 33U,
    PF_Pin2 = (uint16_t) (1 << 15) | 34U, // Doesn't exist.
    PF_Pin3 = (uint16_t) (1 << 15) | 35U, // Doesn't exist.
    PF_Pin4 = (uint16_t) (1 << 15) | 36U,
    PF_Pin5 = (uint16_t) (1 << 15) | 37U,
    PF_Pin6 = (uint16_t) (1 << 15) | 38U,
    PF_Pin7 = (uint16_t) (1 << 15) | 39U,
};

#ifdef ENV_AVR
#include <avr/io.h>

enum pwm_pins
{
    OC0A = (uint8_t) PB_Pin7,
    OC0B = (uint8_t) PD_Pin0
};

volatile uint8_t* port_registers[5];
volatile uint8_t* direction_registers[5];
volatile uint8_t* compare_registers[2];
        /*
        Array of ints containing flag positions for setting pwm inverting mode,
        one each for two compare pins.
        1st bit goes before 0th bit right to left => (1<<COM0X1) | (1<<COM0X0).
        To read this need to find bits set to 1.
        */
        uint8_t pwm_inverting_mode_bits[2];

/*
Translates enums from `pins_mcu` to the corresponding container of pwm inverting
mode bits.
Used by `read_pwm_inverting_mode_bits_offset()` function.
*/
uint8_t resolve_pwm_inverting_mode_bits_offset_container(enum pins_mcu pin_mcu);

/*
Reads the position/offset of the the flags for setting pwm inverting mode.
Use this position number to set the actual flag to either 1 or 0.

Needs the array `inverting_bits_offsets` to store the offsets of the flags.
*/
uint8_t read_pwm_inverting_mode_bits_offset(enum pins_mcu pin_mcu, uint8_t* inverting_bits_offsets);

/*
Reads the actual enum index (0th byte).
Used to get the index for resolving related registers.
*/
uint8_t read_enum_index(enum pins_mcu pin_mcu);

/*
Reads the index of the corresponding pwm comapare register (1st byte).
*/
uint8_t read_compare_reg_index(enum pins_mcu pin_mcu);

/*
Translates enums from `pins_mcu` to the corresponding port register.
*/
volatile uint8_t* resolve_port_reg(enum pins_mcu pin_mcu);

/*
Translates enums from `pins_mcu` to the corresponding direction register.
*/
volatile uint8_t* resolve_direction_reg(enum pins_mcu pin_mcu);

/*
Ensure the enum `pin_mcu` has a pwm register associated.
*/
uint8_t exists_compare_reg(enum pins_mcu pin_mcu);

/*
Translates enums from `pins_mcu` to the corresponding pwm compare register,
if applicable.
*/
volatile uint8_t* resolve_compare_reg(enum pins_mcu pin_mcu);

/*
Translates enums from `pins_mcu` to the corresponding pin offset/number of its port.
E.G. offset 6 with port E = PE_Pin6.
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