#define F_CPU 16000000UL //16 MHz
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

void adc_init(){
	ADMUX |= (1<<REFS0)|(1<<ADLAR); // 5V ref, left adjusted
	// enable adc, enable auto-trigger, enable interrupt & interrupt flag, clock prescaler of 128, start conversion
	ADCSRA |= (1<<ADEN)|(1<<ADATE)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	ADCSRB = 0;
}

void sen_5() {
	ADMUX |= (1<<MUX1)|(1<<MUX0); //enable sensor 5
	ADCSRB |= (1<<MUX5); // free running mode, mux 5 on
	ADCSRA |= (1<<ADSC); // start conversion

	while(!(ADCSRA&(1<<ADIF))){} // Result now available.
	uint16_t pot3 = ADCH;
	if (pot3 > 50) {
		PORTE = (1<<6);
	}
	else{
		PORTE &= ~(1<<6);
	}
		ADCSRA |= (1<<4);
}

void sen_4() {
	ADMUX |= (<<MUX2); // enable sensor 4
	ADCSRA |= (1<<6); // start converstion

	while(!(ADCSRA&(1<<ADIF))){}
	// Result now available.
	uint16_t pot2 = ADCH;
	if (pot2 > 50){
		PORTB = (1<<2);
	}
	else{
		PORTB &= ~(1<<2);
	}
		ADCSRA |= (1<<4);
}

int main(){
	DDRB |= (1<<2); // set LED 0
	DDRE |= (1<<6); // set LED 3
	
	while(1) {
	sen_5();
	sen_4();
	}
}
