#define F_CPU 16000000UL //16 MHz
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define TOLERANCE 125


uint16_t pot8;
uint16_t pot7;
uint16_t pot6;
uint16_t pot5;
uint16_t pot4;
uint16_t pot3;
uint16_t pot2;
uint16_t pot1;

void pwm_init(){
	TCCR0A |= (1<<WGM00);
	TCCR0A |= (1<<WGM01);
	TCCR0A &= ~(1<<WGM02);

	TCCR0B |= (1<<CS02);
	TCCR0B &= ~(1<<CS01);
	TCCR0B &= ~(1<<CS00);

	TCCR0A |= (1<<COM0B1);
	TCCR0A &= ~(1<<COM0B0);

	TCCR0A |= (1<<COM0A1);
	TCCR0A &= ~(1<<COM0A0);
	// motor A uses OCR0A
	// motor A uses OCR0B

	//Motor 2
	DDRB |= (1<<0);
	DDRB |= (1<<7);
	//Motor 1
	DDRE |= (1<<6);
	DDRD |= (1<<0);

	PORTB |= (1<<0); 
	PORTE |= (1<<6);
}

ISR(ADC_vect0)
{
	// Only read the sensor values.
}

void adc_init(){
	ADMUX |= (1<<6)|(1<<5); //5V ref 
	ADCSRA |= (1<<7)|(1<<5)|(1<<4)|(1<<3)|(1<<2)|(1<<1)|1;
	// enable adc, enable auto-trigger, enable interrupt & interrupt flag, clock prescaler of 128, start converion, left adjusted
}

void sen_8() {
	ADCSRB |= (1<<5);	// enable adc 8
	ADCSRA |= (1<<6);	// start conversion

	while(~ADCSRA&(1<<ADIF)){}	// Result now available.
		pot8 = ADCH;
		ADCSRA |= (1<<ADIF);	//clear adif
		ADCSRA &= ~(1<<6);		// stop conversion
		ADCSRB &= ~(1<<5);		// clear admux channels
}

void sen_7() {
	ADMUX |= (1<<0);	// enable adc 9
	ADCSRB |= (1<<5);
	ADCSRA |= (1<<6);	// start conversion

	while(~ADCSRA&(1<<ADIF)){}	// Result now available.
		pot7 = ADCH;
		ADCSRA |= (1<<ADIF);	//clear adif
		ADCSRA &= ~(1<<6);		// stop conversion
		ADMUX &= ~(1<<0);		// clear admux channels
		ADCSRB &= ~(1<<5);		// clear admux channels
}

void sen_6() {
	ADMUX |= (1<<1);	//enable adc10
	ADCSRB |= (1<<5);	// mux 5 on
	ADCSRA |= (1<<6);	// start conversion

	while(~ADCSRA&(1<<ADIF)){}	// Result now available.
		pot6 = ADCH;
		ADCSRA |= (1<<ADIF);	//clear adif
		ADCSRA &= ~(1<<6);		// stop conversion
		ADMUX &= ~(1<<1);		// clear admux channels
		ADCSRB &= ~(1<<5);		// clear admux channels
}

void sen_5() {
	ADMUX |= (1<<1)|(1<<0);	//enable adc11
	ADCSRB |= (1<<5);		// mux 5 on
	ADCSRA |= (1<<6);		// start conversion

	while(~ADCSRA&(1<<ADIF)){}		// Result now available.
		pot5 = ADCH;
		ADCSRA |= (1<<ADIF);		//clear adif
		ADCSRA &= ~(1<<6);			// stop conversion
		ADMUX &= ~((1<<1)|(1<<0));	// clear admux channels
		ADCSRB &= ~(1<<5);			// clear admux channels
}

void sen_4() { 
	ADMUX |= (1<<2)|(1<<1)|(1<<0);			// enable adc7
	ADCSRA |= (1<<6);						// start conversion

	while(~ADCSRA&(1<<ADIF)){}				// Result now available.
		pot4 = ADCH;
		ADCSRA |= (1<<ADIF);				// clear ADIF 
		ADCSRA &= ~(1<<6);					// stop conversion
		ADMUX &= ~((1<<2)|(1<<1)|(1<<0));	// clear admux channels
}

void sen_3() { 
	ADMUX |= (1<<2)|(1<<1);			// enable adc6
	ADCSRA |= (1<<6);				// start conversion

	while(~ADCSRA&(1<<ADIF)){}		// Result now available.
		pot3 = ADCH;
		ADCSRA |= (1<<ADIF);		// clear ADIF 
		ADCSRA &= ~(1<<6);			// stop conversion
		ADMUX &= ~((1<<2)|(1<<1));	// clear admux channels
}

void sen_2() { 
	ADMUX |= (1<<2)|(1<<0);			// enable adc5
	ADCSRA |= (1<<6);				// start conversion

	while(~ADCSRA&(1<<ADIF)){}		// Result now available.
		pot2 = ADCH;
		ADCSRA |= (1<<ADIF);		// clear ADIF 
		ADCSRA &= ~(1<<6);			// stop conversion
		ADMUX &= ~((1<<2)|(1<<0));	// clear admux channels
}

void sen_1() { 
	ADMUX |= (1<<2);				// enable adc4
	ADCSRA |= (1<<6);				// start conversion

	while(~ADCSRA&(1<<ADIF)){}		// Result now available.
		pot1 = ADCH;
		ADCSRA |= (1<<ADIF);		// clear ADIF 
		ADCSRA &= ~(1<<6);			// stop conversion
		ADMUX &= ~(1<<3);			// clear admux channels
}

int main(){

	adc_init();
	pwm_init();

	while(1) {
		sen_8();
		sen_7();
		sen_6();
		sen_5();
		sen_4();
		sen_3();
		sen_2();
		sen_1();

		if (pot4 < TOLERANCE && pot5 < TOLERANCE) { 	// if 4 & 5 see line, go straight
			OCR0A = 120;
			OCR0B = 120;
		}		
		else if(pot4 >= TOLERANCE && pot5 < TOLERANCE){	// if 5 sees line & 4 does not veer left
			OCR0A = 120;
			OCR0B = 110;
		}
		else if(pot4 < TOLERANCE && pot5 >= TOLERANCE){	// if 4 sees line & 5 does not veer right
			OCR0A = 110;
			OCR0B = 120;
		}
		else if(pot6 < TOLERANCE && pot3 < TOLERANCE){	// if 3 & 6 see line go straight (intersection)
			OCR0A = 120;
			OCR0B = 120;
		}
		else if(pot3 < TOLERANCE && pot6 >= TOLERANCE){	// if 3 sees line & 6 does not, stronger veer right
			OCR0A = 80;
			OCR0B = 110;
		}
		else if(pot6 < TOLERANCE && pot3 >= TOLERANCE){	// if 6 sees line & 3 does not, stronger veer left
			OCR0A = 110;
			OCR0B = 80;
		}
		else if(pot7 < TOLERANCE && pot2 >= TOLERANCE){	// if 7 sees line & 2 does not, stop left motor, slow right motor (turn right)
			OCR0A = 100;
			OCR0B = 0;
		}
		else if(pot2 < TOLERANCE && pot7 >= TOLERANCE){	// if 2 sees line & 7 does not, stop right motor, slow left motor (turn left)
			OCR0A = 0;
			OCR0B = 100;
		}
		else{ 
			OCR0A = 0;
			OCR0B = 0;
		}
	}
}
