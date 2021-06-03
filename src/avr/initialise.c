#ifdef ENV_AVR

#include "peripherals.h"
#include "initialise.h"
#include "interrupts.h"
#include "timers.h"
#include "interrupts_timer.h"
#include "serial_comm.h"
#include "leds.h"
#include "motors.h"
#include "sensors.h"

void print_test(void)
{
    uart_putchar('H');
    uart_putchar('i');
    uart_putchar('\r');
    uart_putchar('\n');
    char adc_vals[128];
    serial_comm_send("Hello\r\n", 6);
}

void initialise(void)
{
    // Start motors and pwm:
        // Start pwm:
            // Set up fast pwm with default TOP of 0xFF (255):
    timer0_waveform_generation_mode(fast_pwm_0xFF);
            // Select prescaler:
    timer0_set_prescaler(timer_prescaler256);

        // Setup motors with non-inverting pwm mode
            // Motor A:
    timer0_compare_output_mode(com_clear, comp_A);
        // Motor B:
    timer0_compare_output_mode(com_clear, comp_B);
        // Start motors:
    motor_init();

    // Start leds:
    led_init();

    // Start sensors (starts ADC too):
    sensor_init();

    // Start USART serial communication:
    serial_comm_init(MYUBRR);

    // Start timer for periodic events:
    double tim_lim[1] = { 1000 };
	func_ptr_rvoid_t func[1] = { print_test };
	uint8_t en[1] = { 1 };
    interrupts_timer_init(1, tim_lim, func, en);

    // Start interrupts:
    interrupts_init();
}

#endif //ENV_AVR