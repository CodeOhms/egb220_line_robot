#define F CPU_16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
  
int main(void)
{
    DDRB |= (1<<0);            // initialize port B
    while(1)
    {

    // clockwise rotation
    PORTB = ((1<<0) | (1<<2)); // set PORTB = 0b00000101;
							   // set pin 0 to 1
							   // set pin 2 to 1
    _delay_ms(500);            // wait 0.5s
  
	PORTB &= ~((1<<0) | (1<<2)); // clear bits

    // anti-clockwise rotation
    PORTB = ((1<<1) | (1<<3)); // set PORTB = 0b00001010;
	  						   // set pin 1 to 1
	  						   // set pin 3 to 1
    _delay_ms(500);            // wait 0.5s

	PORTB &= ~((1<<1) | (1<<3)); // clear bits
    }
}
