#ifndef  SENSORS_H
#define  SENSORS_H

#if ENV_AVR == 1

#include "adc.h"

extern const uint8_t _channels[SENSORS_NUM];

void sensor_init();

void sensor_close();

/*
Call this function to read the next sensor as specified in the `` array.

Returns:
The result of the sensor reading.
*/
uint16_t sensor_read();

#endif // ENV_AVR == 1 

#endif //SENSORS_H