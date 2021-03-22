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


#ifdef ENV_AVR
#include <avr/io.h>

enum pwm_pins
{
    OC0A = (uint8_t) PB_Pin7,
    OC0B = (uint8_t) PD_Pin0
};

typedef struct _pin_extra_info
{
    uint8_t compare_reg_index;
    /*
    Array of ints containing flag positions for setting pwm inverting mode,
    one each for two compare pins.
    1st bit goes before 0th bit right to left => (1<<COM0X1) | (1<<COM0X0).
    To read this need to find bits set to 1.
    */
    uint8_t pwm_inverting_mode_bits_offsets;
} pin_extra_info;

pin_extra_info info_PB_Pin7;
pin_extra_info info_PD_Pin0;
pin_extra_info info_PB_Pin5;
pin_extra_info info_PB_Pin6;

extern pin_extra_info* pins_extra_info[40];

volatile uint8_t* port_registers[5];
volatile uint8_t* direction_registers[5];
volatile uint8_t* compare_registers[4];

/*
Reads the position/offset of the the flags for setting pwm inverting mode.
Use this position number to set the actual flag to either 1 or 0.

Needs the array `inverting_bits_offsets` to store the offsets of the flags.
*/
uint8_t read_pwm_inverting_mode_bits_offset(enum pins_mcu pin_mcu, uint8_t* inverting_bits_offsets);

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