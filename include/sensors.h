#ifndef  SENSORS_H
#define  SENSORS_H

#if ENV_AVR == 1

#include "adc.h"

void sensors_init();

#endif // ENV_AVR == 1 

#endif //SENSORS_H