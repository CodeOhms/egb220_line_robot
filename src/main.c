#include "initialise.h"
#include "motors.h"

#ifdef ENV_AVR

#include <util/delay.h>

void delay(double ms)
{
    _delay_ms(ms);
}

#endif //ENV_AVR

// Use this as our "int main(void)" replacement.
void line_following_robot(void)
{
    initialise();

    while(1)
    {
        // Test motors.
        motor_move(100.0, reverse, motor_left);
        motor_move(100.0, reverse, motor_right);
        delay(5000);
        motor_move(10.0, reverse, motor_left);
        motor_move(10.0, reverse, motor_right);
        delay(5000);

        // motor_move(100.0, reverse, motor_left);
        // motor_move(100.0, reverse, motor_right);
        // delay(5000);

        // motor_move(10.0, reverse, motor_left);
        // motor_move(10.0, reverse, motor_right);
        // delay(5000);
    }

    // Do not allow this function to return!
}

#ifdef ENV_AVR

int main(void)
{
    line_following_robot();

    return 0;
}

#endif //ENV_AVR