#include "initialise.h"
#include "peripherals.h"
#include "timers.h"
#include "interrupts_timer.h"
#include "interrupts.h"
#include "serial_comm.h"
#include "leds.h"
#include "motors.h"
#include "sensors.h"
#include "main.h"

#include <stdio.h>
#include <avr/interrupt.h>

#ifdef ENV_AVR

#include <util/delay.h>

void delay(double ms)
{
    _delay_ms(ms);
}

#endif //ENV_AVR

uint16_t sensor_readings[SENSORS_NUM];

// Use this as our "int main(void)" replacement.
void line_following_robot(void)
{
    while(1)
    {
        char adc_vals[128];
        // Sample from all sensors:
        for(uint8_t s = 0; s < SENSORS_NUM; ++s)
        {
            sensor_readings[s] = sensor_read(s);
            // sprintf(adc_vals, "Sensor %d = %d\r\n", s, sensor_readings[s]);
            // serial_comm_send(adc_vals, 128);
            // uart_putchar('h');
            // uart_putchar('\r');
            // uart_putchar('\n');
        }
    }

    // Do not allow this function to return, otherwise `main()` will end!
}

#ifdef ENV_AVR

int main(void)
{
    initialise();
    line_following_robot();

    return 0;
}

#endif //ENV_AVR