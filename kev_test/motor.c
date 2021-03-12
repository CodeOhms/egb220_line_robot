#define F CPU_16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
  
int main(void)
{
    DDRC |= (1<<0);            // initialize port C
    while(1)
    {

    // clockwise rotation
    PORTC = (1<<0) | (1<<2); // set PORTC = 0b00000101;
    _delay_ms(500);         // wait 0.5s
  

    // anti-clockwise rotation
    PORTC = (1<<1) | (1<<4); // set PORTC = 0b00001010;
    _delay_ms(500);         // wait 0.5s
    }
}
