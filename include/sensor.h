#ifndef  SENSOR_H
#define  SENSOR_H

#if ENV_AVR == 1

#include "adc.h"

void sensor_init();

void sensor_read();

#endif // ENV_AVR == 1 

#endif //SENSOR_H