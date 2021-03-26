#define F_CPU 16000000UL //16 MHz
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

uint16_t pot4;
uint16_t pot5;

void pwm_init(){
	// motor A uses OCR0A
	// motor A uses OCR0B

	// fast PWM mode
	TCCR0A &= ~(1<<WGM02);
	TCCR0A |= (1<<WGM01);
	TCCR0A |= (1<<WGM00);

	// clk/1024 from prescaler
	TCCR0B |= (1<<CS02);
	TCCR0B &= ~(1<<CS01); 
	TCCR0B &= ~(1<<CS00);
	
	TCCR0A |= (1<<COM0B1); 
	TCCR0A &= ~(1<<COM0B0);

	TCCR0A |= (1<<COM0A1); 
	TCCR0A &= ~(1<<COM0A0);

}

ISR(ADC_vect0)
{
  // Only read the sensor values.
}

void adc_init() {
	ADMUX |= (1<<REFS0)|(1<<ADLAR); // 5V ref, left adjusted
	// enable adc, enable auto-trigger, enable interrupt & interrupt flag, clock prescaler of 128
	ADCSRA |= (1<<ADEN)|(1<ADATE)|(1<<ADIF)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	ADCSRB = 0; // free running mode
}

void sen_5() {
	ADMUX |= (1<<MUX1)|(1<<MUX0); // enable ADC11
	ADCSRB |= (1<<MUX5); 

	ADCSRA |= (1<<ADSC); // start conversion

	while(~ADCSRA&(1<<ADIF)){} // Result now available.
	pot5 = ADCH;
	ADCSRA |= (1<<ADIF); //clear adif
	ADCSRA &= ~(1<<ADSC); // stop conversion
	ADMUX &= ~(1<<MUX1)|(1<<MUX0); // clear admux channels
	ADCSRB &= ~(1<<MUX5); 
}

void sen_4() {
	ADMUX |= (1<<MUX2)|(1<<MUX1)|(1<<MUX0); // enable ADC7 

	ADCSRA |= (1<<ADSC); // start conversion

	while(~ADCSRA&(1<<ADIF)){}
	pot4 = ADCH;
	ADCSRA |= (1<<ADIF); // clear ADIF 
	ADCSRA &= ~(1<<ADSC); // stop conversion
	ADMUX &= ~(1<<MUX2)|(1<<MUX1)|(1<<MUX0); // clear admux channels
}

int main(){

	// Set Motor A
	DDRB |= (1<<0);
	DDRB |= (1<<7);
	// Set Motor B
	DDRE |= (1<<6); 
	DDRD |= (1<<0);

	pwm_init();

	while(1) {
		sen_5();
		sen_4();
		// if both sensors see the line, go half speed.
		if (pot4 < 237 && pot5 < 237) {
			PORTB |= (1<<0); 
			OCR0A = 127; 

			PORTE |= (1<<6);
			OCR0B = 127; 
		}		
		// If sensor 4 does not see line & sensor 5 does, speed up motor-a, slow down motor-b
		else if(pot4 >= 237 && pot5 < 237){
			PORTB |= (1<<0); 
			OCR0A = 132; 

			PORTE |= (1<<6);
			OCR0B = 122;
		}
		// If sensor 4 sees line & sensor 5 does not, slow down motor-a, speed up motor-b
		else if(pot4 < 237 && pot5 >= 237){
			PORTB |= (1<<0);
			OCR0A = 122; 

			PORTE |= (1<<6);
			OCR0B = 132;
		}
		
		else{ // if neither sensor sees the line, turn off
			PORTB &= ~(1<<0);
			//PORTB &= ~(1<<7);
			OCR0A = 0;

			PORTE &= ~(1<<6); 
			//PORTD &= ~(1<<0);
			OCR0B = 0;
		}
	}
}
