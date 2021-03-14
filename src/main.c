#if ENV_AVR == 1

#include "initialise.h"

// Use this as our "int main(void)" replacement.
int line_following_robot(void)
{
    initialise();

    while(1)
    {
        
    }

    return 0; // Don't ever let this occur!
}

#endif //ENV_AVR