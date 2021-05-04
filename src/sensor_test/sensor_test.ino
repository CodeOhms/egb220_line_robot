#define F_CPU 16000000UL //16 MHz
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define TOLERANCE 140


uint16_t pot8;
uint16_t pot7;
uint16_t pot6;
uint16_t pot5;
uint16_t pot4;
uint16_t pot3;
uint16_t pot2;
uint16_t pot1;



ISR(ADC_vect0)
{
	// Only read the sensor values.
}

void adc_init(){
	ADMUX |= (1<<6)|(1<<5); //5V ref 
	ADCSRA |= (1<<7)|(1<<5)|(1<<4)|(1<<3)|(1<<2)|(1<<1);
	// enable adc, enable auto-trigger, enable interrupt & interrupt flag, clock prescaler of 64, start converion, left adjusted
}

void sen_8() {
	ADCSRB |= (1<<5);	// enable adc 8 -> 100000
	while(~ADCSRA&(1<<ADIF)){}	// Result now available.
		pot8 = ADCH;
		ADCSRA |= (1<<ADIF);	//clear adif
		ADCSRB &= ~(1<<5);		// clear admux channels
}

void sen_7() {
	ADMUX |= (1<<0);	// enable adc 9 -> 100001
	ADCSRB |= (1<<5);
	while(~ADCSRA&(1<<ADIF)){}	// Result now available.
		pot7 = ADCH;
		ADCSRA |= (1<<ADIF);	//clear adif
		ADMUX &= ~(1<<0);		// clear admux channels
		ADCSRB &= ~(1<<5);		// clear admux channels
}

void sen_6() {
	ADMUX |= (1<<1);	//enable adc10 -> 100010
	ADCSRB |= (1<<5);	// mux 5 on
	while(~ADCSRA&(1<<ADIF)){}	// Result now available.
		pot6 = ADCH;
		ADCSRA |= (1<<ADIF);	//clear adif
		ADMUX &= ~(1<<1);		// clear admux channels
		ADCSRB &= ~(1<<5);		// clear admux channels
}

void sen_5() {
	ADMUX |= (1<<1)|(1<<0);	//enable adc11 -> 100011
	ADCSRB |= (1<<5);		// mux 5 on
	while(~ADCSRA&(1<<ADIF)){}		// Result now available.
		pot5 = ADCH;
		ADCSRA |= (1<<ADIF);		//clear adif
		ADMUX &= ~((1<<1)|(1<<0));	// clear admux channels
		ADCSRB &= ~(1<<5);			// clear admux channels
}

void sen_4() { 
	ADMUX |= (1<<2)|(1<<1)|(1<<0);			// enable adc7 -> 000111
	while(~ADCSRA&(1<<ADIF)){}				// Result now available.
		pot4 = ADCH;
		ADCSRA |= (1<<ADIF);				// clear ADIF 
		ADMUX &= ~((1<<2)|(1<<1)|(1<<0));	// clear admux channels
}

void sen_3() { 
	ADMUX |= (1<<2)|(1<<1);			// enable adc6 -> 000110
	while(~ADCSRA&(1<<ADIF)){}		// Result now available.
		pot3 = ADCH;
		ADCSRA |= (1<<ADIF);		// clear ADIF 
		ADMUX &= ~((1<<2)|(1<<1));	// clear admux channels
}

void sen_2() { 
	ADMUX |= (1<<2)|(1<<0);			// enable adc5 -> 000101
	while(~ADCSRA&(1<<ADIF)){}		// Result now available.
		pot2 = ADCH;
		ADCSRA |= (1<<ADIF);		// clear ADIF 
		ADMUX &= ~((1<<2)|(1<<0));	// clear admux channels
}

void sen_1() { 
	ADMUX |= (1<<2);				// enable adc4 -> 000100
	while(~ADCSRA&(1<<ADIF)){}		// Result now available.
		pot1 = ADCH;
		ADCSRA |= (1<<ADIF);		// clear ADIF 
		ADMUX &= ~(1<<2);			// clear admux channels
}



int main(){

	adc_init();
	while(1) {
		ADCSRA |= (1<<6);	// start conversion

		sen_8(); 
		sen_7(); 
		sen_6(); 
		sen_5(); 
		sen_4(); 
		sen_3(); 
		sen_2(); 
		sen_1(); 

		ADCSRA &= ~(1<<6);		// stop conversion
		if(	pot1 < TOLERANCE || pot2 < TOLERANCE ||
			pot3 < TOLERANCE || pot4 < TOLERANCE ||
			pot5 < TOLERANCE || pot6 < TOLERANCE ||
			pot7 < TOLERANCE || pot8 < TOLERANCE
		)
		{
		DDRE |= (1<<6);
		PORTE |= (1<<6); 
		}
		else{PORTE &= ~(1<<6);}
	}
}
