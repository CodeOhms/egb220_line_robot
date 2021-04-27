#define F_CPU 16000000UL //16 MHz
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define TOLERANCE 130

uint16_t pot3;
uint16_t pot4;
uint16_t pot5;
uint16_t pot6;

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
}

ISR(ADC_vect0)
{
  // Only read the sensor values.
}

void sen_6() {
	ADMUX |= (1<<6)|(1<<5)|(1<<1); //enable adc10, 5V ref, left adjusted
	// enable adc, enable auto-trigger, enable interrupt & interrupt flag, clock prescaler of 128, start converion
	ADCSRA |= (1<<7)|(1<<5)|(1<<4)|(1<<3)|(1<<2)|(1<<1);
	ADCSRB |= (1<<5); // free running mode, mux 5 on
	ADCSRA |= (1<<6); // start conversion

	while(~ADCSRA&(1<<ADIF)){} // Result now available.
	pot6 = ADCH;
		ADCSRA |= (1<<ADIF); //clear adif
		ADCSRA &= ~(1<<6); // stop conversion
		ADMUX &= ~(1<<1); // clear admux channels
		ADCSRB &= ~(1<<5); // clear admux channels
}

void sen_5() {
	ADMUX |= (1<<6)|(1<<5)|(1<<1)|(1<<0); //enable adc11, 5V ref, left adjusted
	// enable adc, enable auto-trigger, enable interrupt & interrupt flag, clock prescaler of 128, start conversion
	ADCSRA |= (1<<7)|(1<<5)|(1<<4)|(1<<3)|(1<<2)|(1<<1)|1;
	ADCSRB |= (1<<5); // free running mode, mux 5 on
	ADCSRA |= (1<<6); // start conversion

	while(~ADCSRA&(1<<ADIF)){} // Result now available.
	pot5 = ADCH;
		ADCSRA |= (1<<ADIF); //clear adif
		ADCSRA &= ~(1<<6); // stop conversion
		ADMUX &= ~((1<<1)|(1<<0)); // clear admux channels
		ADCSRB &= ~(1<<5); // clear admux channels
}

void sen_4() { // Start single conversion by setting ADSC bit in ADCSRA
	ADMUX |= (1<<6)|(1<<5)|(1<<2)|(1<<1)|(1<<0); // enable adc7, 5V ref. left adjusted.
	// enable adc, enable auto-trigger, enable interrupt & interrupt flag, clock prescaler of 128
	ADCSRA |= (1<<7)|(1<<5)|(1<<4)|(1<<3)|(1<<2)|(1<<1)|1;
	ADCSRB = 0; // free running mode
	ADCSRA |= (1<<6); // start conversion

	while(~ADCSRA&(1<<ADIF)){}
	// Result now available.
	  pot4 = ADCH;
		ADCSRA |= (1<<ADIF); // clear ADIF 
		ADCSRA &= ~(1<<6); // stop conversion
		ADMUX &= ~((1<<2)|(1<<1)|(1<<0)); // clear admux channels
}
void sen_3() { // Start single conversion by setting ADSC bit in ADCSRA
	ADMUX |= (1<<6)|(1<<5)|(1<<2)|(1<<1); // enable adc6, 5V ref. left adjusted.
	// enable adc, enable auto-trigger, enable interrupt & interrupt flag, clock prescaler of 128
	ADCSRA |= (1<<7)|(1<<5)|(1<<4)|(1<<3)|(1<<2)|(1<<1)|1;
	ADCSRB = 0; // free running mode
	ADCSRA |= (1<<6); // start conversion

	while(~ADCSRA&(1<<ADIF)){}
	// Result now available.
	  pot3 = ADCH;
		ADCSRA |= (1<<ADIF); // clear ADIF 
		ADCSRA &= ~(1<<6); // stop conversion
		ADMUX &= ~((1<<2)|(1<<1)); // clear admux channels
}

int main(){

	//Motor 2
	DDRB |= (1<<0); // Set Bit/Pin 0 to 1/Output
	DDRB |= (1<<7); // Set Bit/Pin 0 to 1/Output
	//Motor 1
	DDRE |= (1<<6); // Set Bit/Pin 0 to 1/Output
	DDRD |= (1<<0); // Set Bit/Pin 0 to 1/Output

	pwm_init();

	while(1) {
		sen_6();
		sen_5();
		sen_4();
		sen_3();

		if (pot4 < TOLERANCE && pot5 < TOLERANCE) { // If 4 & 5 see line, go straight
			PORTB |= (1<<0); // Set Bit/Pin 0 to 1/Output
			OCR0A = 100; // Set Bit/Pin 0 to 1/Output

			PORTE |= (1<<6); // Set Bit/Pin 0 to 1/Output
			OCR0B = 100;
		}		
		else if(pot4 >= TOLERANCE && pot5 < TOLERANCE){ // if 5 sees line and 4 does not veer left
			PORTB |= (1<<0); // Set Bit/Pin 0 to 1/Output
			OCR0A = 100; // Set Bit/Pin 0 to 1/Output

			PORTE |= (1<<6); // Set Bit/Pin 0 to 1/Output
			OCR0B = 98;
		}
		else if(pot4 < TOLERANCE && pot5 >= TOLERANCE){ // if 4 sees line and 5 does not veer right
			PORTB |= (1<<0); // Set Bit/Pin 0 to 1/Output
			OCR0A = 98; // Set Bit/Pin 0 to 1/Output

			PORTE |= (1<<6); // Set Bit/Pin 0 to 1/Output
			OCR0B = 100;
		}
		
		else if(pot6 < TOLERANCE && pot3 < TOLERANCE){ // if 3 & 6 see line go straight (intersection)
			PORTB |= (1<<0); // Set Bit/Pin 0 to 1/Output
			OCR0A = 100; // Set Bit/Pin 0 to 1/Output

			PORTE |= (1<<6); // Set Bit/Pin 0 to 1/Output
			OCR0B = 100;
		}
		else if(pot6 < TOLERANCE && pot3 >= TOLERANCE){ // if 6 sees line and 3 does not turn off motor A, slow down motor B (turn left)
			PORTB |= (1<<0); // Set Bit/Pin 0 to 1/Output
			OCR0A = 0; // Set Bit/Pin 0 to 1/Output

			PORTE |= (1<<6); // Set Bit/Pin 0 to 1/Output
			OCR0B = 50;
		}
		else if(pot3 < TOLERANCE && pot6 >= TOLERANCE){ // if 3 sees line and 6 does not turn off motor B, slow down motor A (turn right)
			PORTB |= (1<<0); // Set Bit/Pin 0 to 1/Output
			OCR0A = 50; // Set Bit/Pin 0 to 1/Output

			PORTE |= (1<<6); // Set Bit/Pin 0 to 1/Output
			OCR0B = 0;
		}
		else{ // slow down
			PORTB |= (1<<0); // Set Bit/Pin 0 to 1/Output
			OCR0A = 0; // Set Bit/Pin 0 to 1/Output

			PORTE |= (1<<6); // Set Bit/Pin 0 to 1/Output
			OCR0B = 0;
		}
	}
}
