#if ENV_AVR == 1

#include "initialise.h"
#include "motors.h"

// Use this as our "int main(void)" replacement.
void line_following_robot(void)
{
    initialise();

    while(1)
    {
        
    }

    return 0; // Don't ever let this occur!
    // Test motors.
    motor_move(100.0, forward, motor_left);
    motor_move(100.0, forward, motor_right);
    delay(5000);
    motor_move(10.0, reverse, motor_left);
    motor_move(100.0, reverse, motor_right);
    delay(5000);
}

#endif //ENV_AVR