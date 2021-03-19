#define F_CPU 16000000UL
#include <avr/io.h>

int main() {
	//Motor 2
	DDRB |= (1<<0); // Set Bit/Pin 0 to 1/Output
	DDRB |= (1<<7); // Set Bit/Pin 0 to 1/Output
	//Motor 1
	DDRE |= (1<<6); // Set Bit/Pin 0 to 1/Output
	DDRD |= (1<<0); // Set Bit/Pin 0 to 1/Output

	// 2
	PORTB |= (1<<0); // Set Bit/Pin 0 to 1/Output
	PORTB |= (1<<7); // Set Bit/Pin 0 to 1/Output

	PORTE |= (1<<6); // Set Bit/Pin 0 to 1/Output
	PORTD |= (1<<0); // Set Bit/Pin 0 to 1/Output

	return 0;
}
