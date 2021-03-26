#define F_CPU 16000000UL //16 MHz
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

uint16_t pot3;
uint16_t pot2;

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

void sen_5() {
	ADMUX |= (1<<6)|(1<<5)|(1<<1)|(1<<0); //enable sensor 5, 5V ref, left adjusted
	// enable adc, enable auto-trigger, enable interrupt & interrupt flag, clock prescaler of 128, start conversion
	ADCSRA |= (1<<7)|(1<<5)|(1<<4)|(1<<3)(1<<2)|(1<<1)|1;
	ADCSRB |= (1<<5); // free running mode, mux 5 on
	ADCSRA |= (1<<6); // start conversion

	while(~ADCSRA&(1<<ADIF)){} // Result now available.
	pot2 = ADCH;
	//if (pot2 < 30) {
	//	PORTE = (1<<6);
	//}
	//else{
	//	PORTE &= ~(1<<6);
	//}
		ADCSRA |= (1<<ADIF); //clear adif
		ADCSRA &= ~(1<<6); // stop conversion
		ADMUX &= ~(1<<1)|(1<<0); // clear admux channels
		ADCSRB &= ~(1<<5); // clear admux channels
}

void sen_4() {
   // Start single conversion by setting ADSC bit in ADCSRA
	ADMUX |= (1<<6)|(1<<5)|(1<<2)|(1<<1)|(1<<0); // enable sensor 4, 5V ref. left adjusted.
	// enable adc, enable auto-trigger, enable interrupt & interrupt flag, clock prescaler of 128
	ADCSRA |= (1<<7)|(1<<5)|(1<<4)|(1<<3)|(1<<2)|(1<<1)|1;
	ADCSRB = 0; // free running mode
	ADCSRA |= (1<<6); // start conversion

	while(~ADCSRA&(1<<ADIF)){}
	// Result now available.
	  pot3 = ADCH;
	//if (pot3 < 30){
	//	PORTB = (1<<2);
	//	ADCSRA |= (0<<ADIF);
	//}
	//else{
	//	PORTB &= ~(1<<2);
	//}
		ADCSRA |= (1<<ADIF); // clear ADIF 
		ADCSRA &= ~(1<<6); // stop conversion
		ADMUX &= ~(1<<2)|(1<<1)|(1<<1); // clear admux channels
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
		sen_5();
		sen_4();

		if (pot3 < 237 && pot2 < 237) {
			PORTB |= (1<<0); // Set Bit/Pin 0 to 1/Output
			OCR0A = 127; // Set Bit/Pin 0 to 1/Output

			PORTE |= (1<<6); // Set Bit/Pin 0 to 1/Output
			OCR0B = 127;
		}		
		else if(pot3 >= 237 && pot2 < 237){
			PORTB |= (1<<0); // Set Bit/Pin 0 to 1/Output
			OCR0A = 130; // Set Bit/Pin 0 to 1/Output

			PORTE |= (1<<6); // Set Bit/Pin 0 to 1/Output
			OCR0B = 120;
		}
		else if(pot3 < 237 && pot2 >= 237){
			PORTB |= (1<<0); // Set Bit/Pin 0 to 1/Output
			OCR0A = 120; // Set Bit/Pin 0 to 1/Output

			PORTE |= (1<<6); // Set Bit/Pin 0 to 1/Output
			OCR0B = 130;
		}
		
		else{
			PORTB &= ~(1<<0); // Set Bit/Pin 0 to 1/Output
			//PORTB &= ~(1<<7); // Set Bit/Pin 0 to 1/Output
			OCR0A = 0; // Set Bit/Pin 0 to 1/Output

			PORTE &= ~(1<<6); // Set Bit/Pin 0 to 1/Output
			//PORTD &= ~(1<<0); // Set Bit/Pin 0 to 1/Output
			OCR0B = 0;
		}
	}
}
