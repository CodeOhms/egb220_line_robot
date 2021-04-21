#define F_CPU 16000000UL //16 MHz
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

uint16_t pot3;
uint16_t pot4;
uint16_t pot5;
uint16_t pot6;

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
	// enable adc, enable auto-trigger, enable interrupt & interrupt flag, clock prescaler f 128
	ADCSRA |= (1<<ADEN)|(1<<ADATE)|(1<<ADIF)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	ADCSRB = 0; // free running mode
}

void sen_6() {
	ADMUX |= (1<<MUX1); // enable ADC10
	ADCSRB |= (1<<MUX5); 

	ADCSRA |= (1<<ADSC); // start conversion

	while(~ADCSRA&(1<<ADIF)){} // Result now available.
	pot6 = ADCH;
	ADCSRA |= (1<<ADIF); //clear adif
	ADCSRA &= ~(1<<ADSC); // stop conversion
	ADMUX &= ~(1<<MUX1); // clear admux channels
	ADCSRB &= ~(1<<MUX5); 
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

void sen_3() {
	ADMUX |= (1<<MUX2)|(1<<MUX0); // enable ADC7 

	ADCSRA |= (1<<ADSC); // start conversion

	while(~ADCSRA&(1<<ADIF)){}
	pot5 = ADCH;
	ADCSRA |= (1<<ADIF); // clear ADIF 
	ADCSRA &= ~(1<<ADSC); // stop conversion
	ADMUX &= ~(1<<MUX2)|(1<<MUX0); // clear admux channels
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
		sen_6();
		sen_5();
		sen_4();
		sen_3();
		// If both sensors see the line, go half speed.
		else if (pot4 < 237 && pot5 < 237) {
			PORTB |= (1<<0); 
			OCR0A = 150; 

			PORTE |= (1<<6);
			OCR0B = 150; 
		}		
		// If sensor 4 does not see line & sensor 5 does slow down motor-b
		else if(pot4 >= 237 && pot5 < 237){
			PORTB |= (1<<0); 
			OCR0A = 150; 

			PORTE |= (1<<6);
			OCR0B = 145;
		}
		// If sensor 4 sees line & sensor 5 does not, slow down motor-a
		else if(pot4 < 237 && pot5 >= 237){
			PORTB |= (1<<0);
			OCR0A = 145; 

			PORTE |= (1<<6);
			OCR0B = 150;
		}

		else if(pot3 < 237){
			PORTB |= (1<<0);
			OCR0A = 125; 

			PORTE |= (1<<6);
			OCR0B = 145;
		}
		else if(pot4 < 237){
			PORTB |= (1<<0);
			OCR0A = 145; 

			PORTE |= (1<<6);
			OCR0B = 125;
		}
		
		else{ // if neither sensor sees the line, turn off
			PORTB |= (1<<0);
			PORTB &= ~(1<<7);
			//OCR0A = 0;

			PORTE &= ~(1<<6); 
			//PORTD &= ~(1<<0);
			OCR0B = 1;
		}
	}
}

for ( int i; i < 8; i++ ) {
		pot[i] = adcread(i);
		if (sensor_data[i] > 235) {
			sensor_bool[i] = 1;
		} else {
			sensor_bool[i] = 0;
		}
	}

	if (sensor_bool[0] == 1) {
		position = position + 1;
	}
	if (sensor_boolean[1] == 1) {
		position = position + 1;
	}
	if (sensor_boolean[2] == 1) {
		position = position + 1;
	}
	if (sensor_boolean[3] == 1) {
		position = position + 1;
	}

	if (sensor_boolean[4] == 1) {
		position = position - 1;
	}
	if (sensor_boolean[5] == 1) {
		position = position - 1;
	}
	if (sensor_boolean[6] == 1) {
		position = position - 1;
	}
	if (sensor_boolean[7] == 1) {
		position = position - 1;
	}

