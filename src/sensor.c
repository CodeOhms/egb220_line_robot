#define F_CPU 16000000UL //16 MHz
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

void sen_5() {
	ADMUX |= (1<<6)|(1<<5)|(1<<1)|(1<<0); //enable sensor 5, 5V ref, left adjusted
	// enable adc, enable auto-trigger, enable interrupt & interrupt flag, clock prescaler of 128, start conversion
	ADCSRA |= (1<<7)|(1<<5)|(1<<3)|(1<<2)|(1<<1)|1;
	ADCSRB |= (1<<5); // free running mode, mux 5 on
	ADCSRA |= (1<<6); // start conversion

	while(~ADCSRA&(1<<ADIF)){} // Result now available.
	uint16_t pot3 = ADCH;
	if (pot3 < 50) {
		PORTE = (1<<6);
	}
	else{
		PORTE &= ~(1<<6);
	}
		ADCSRA |= (0<<ADIF);
}
void sen_4() {
   // Start single conversion by setting ADSC bit in ADCSRA
	ADMUX |= (1<<6)|(1<<5)|(1<<2); // enable sesor 4, 5V ref. left adjusted.
	// enable adc, enable auto-trigger, enable interrupt & interrupt flag, clock prescaler of 128, start conversion
	ADCSRA |= (1<<7)|(1<<5)|(1<<3)|(1<<2)|(1<<1)|1;
	ADCSRB = 0; // free running mode
	ADCSRA |= (1<<6); // start converstion

	while(~ADCSRA&(1<<ADIF)){}
	// Result now available.
	uint16_t pot2 = ADCH;
	if (pot2 < 50){
		PORTB = (1<<2);
		ADCSRA |= (0<<ADIF);
	}
	else{
		PORTB &= ~(1<<2);
	}
		ADCSRA |= (0<<ADIF);
}
int main(){
	DDRB |= (1<<0); // set LED 0
	DDRE |= (1<<6); // set LED 3
	
	while(1) {
	sen_5();
	sen_4();
	}
}
