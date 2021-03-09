#include "include/initialise.h"

#ifdef    ENV_ARDUINO
#include <Arduino.h>
#include "include/main.h"

void setup(void)
{
    // Leave this empty so we can use the 'initialise(void)' function instead.
}

void initialise(void)
{

}

/*
Have this function pass control over to 'line_following_robot(void)' function.
This function should never pass control back.
*/
void loop(void)
{
    line_following_robot();
}

#endif //ENV_ARDUINO


#ifdef   ENV_AVR

void initialise(void)
{

}

#endif //ENV_AVR