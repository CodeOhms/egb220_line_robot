#define F_CPU 16000000UL //16 MHz
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define TOLERANCE 70
#define BASE 40
#define Kp 13
#define Kd 0.1

uint16_t pot8;
uint16_t pot7;
uint16_t pot6;
uint16_t pot5;
uint16_t pot4;
uint16_t pot3;
uint16_t pot2;
uint16_t pot1;

int error;
int *last_error = 0;

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
	ADCSRA |= (1<<7)|(1<<5)|(1<<4)|(1<<3)|(1<<2)|(1<<1);
	// enable adc, enable auto-trigger, enable interrupt & interrupt flag, clock prescaler of 128, start converion, left adjusted
}

void sen_8() {
	ADCSRB |= (1<<5);	// enable adc 8 -> 100000
	ADCSRA |= (1<<6);	// start conversion

	while(~ADCSRA&(1<<ADIF)){}	// Result now available.
		pot8 = ADCH;
		ADCSRA |= (1<<ADIF);	//clear adif
		ADCSRB &= ~(1<<5);		// clear admux channels
		ADCSRA &= ~(1<<6);		// stop conversion
}

void sen_7() {
	ADMUX |= (1<<0);	// enable adc 9 -> 100001
	ADCSRB |= (1<<5);
	ADCSRA |= (1<<6);	// start conversion

	while(~ADCSRA&(1<<ADIF)){}	// Result now available.
		pot7 = ADCH;
		ADCSRA |= (1<<ADIF);	//clear adif
		ADMUX &= ~(1<<0);		// clear admux channels
		ADCSRB &= ~(1<<5);		// clear admux channels
		ADCSRA &= ~(1<<6);		// stop conversion
}

void sen_6() {
	ADMUX |= (1<<1);	//enable adc10 -> 100010
	ADCSRB |= (1<<5);	// mux 5 on
	ADCSRA |= (1<<6);	// start conversion

	while(~ADCSRA&(1<<ADIF)){}	// Result now available.
		pot6 = ADCH;
		ADCSRA |= (1<<ADIF);	//clear adif
		ADMUX &= ~(1<<1);		// clear admux channels
		ADCSRB &= ~(1<<5);		// clear admux channels
		ADCSRA &= ~(1<<6);		// stop conversion
}

void sen_5() {
	ADMUX |= (1<<1)|(1<<0);	//enable adc11 -> 100011
	ADCSRB |= (1<<5);		// mux 5 on
	ADCSRA |= (1<<6);	// start conversion

	while(~ADCSRA&(1<<ADIF)){}		// Result now available.
		pot5 = ADCH;
		ADCSRA |= (1<<ADIF);		//clear adif
		ADMUX &= ~((1<<1)|(1<<0));	// clear admux channels
		ADCSRB &= ~(1<<5);			// clear admux channels
		ADCSRA &= ~(1<<6);		// stop conversion
}

void sen_4() { 
	ADMUX |= (1<<2)|(1<<1)|(1<<0);			// enable adc7 -> 000111
	ADCSRA |= (1<<6);	// start conversion

	while(~ADCSRA&(1<<ADIF)){}				// Result now available.
		pot4 = ADCH;
		ADCSRA |= (1<<ADIF);				// clear ADIF 
		ADMUX &= ~((1<<2)|(1<<1)|(1<<0));	// clear admux channels
		ADCSRA &= ~(1<<6);		// stop conversion
}

void sen_3() { 
	ADMUX |= (1<<2)|(1<<1);			// enable adc6 -> 000110
	ADCSRA |= (1<<6);	// start conversion

	while(~ADCSRA&(1<<ADIF)){}		// Result now available.
		pot3 = ADCH;
		ADCSRA |= (1<<ADIF);		// clear ADIF 
		ADMUX &= ~((1<<2)|(1<<1));	// clear admux channels
		ADCSRA &= ~(1<<6);		// stop conversion
}

void sen_2() { 
	ADMUX |= (1<<2)|(1<<0);			// enable adc5 -> 000101
	ADCSRA |= (1<<6);	// start conversion

	while(~ADCSRA&(1<<ADIF)){}		// Result now available.
		pot2 = ADCH;
		ADCSRA |= (1<<ADIF);		// clear ADIF 
		ADMUX &= ~((1<<2)|(1<<0));	// clear admux channels
		ADCSRA &= ~(1<<6);		// stop conversion
}

void sen_1() { 
	ADMUX |= (1<<2);				// enable adc4 -> 000100
	ADCSRA |= (1<<6);	// start conversion

	while(~ADCSRA&(1<<ADIF)){}		// Result now available.
		pot1 = ADCH;
		ADCSRA |= (1<<ADIF);		// clear ADIF 
		ADMUX &= ~(1<<2);			// clear admux channels
		ADCSRA &= ~(1<<6);		// stop conversion
}

void setMotorSpeeds(double motorA, double motorB) {
	OCR0A = 255 * motorA/100; 
	OCR0B = 255 * motorB/100;
}

int current_position(){
	int position = 0;

	if(pot1 < TOLERANCE && pot8 < TOLERANCE){position = 0;} // Straight line case
	if(pot4 < TOLERANCE && pot5 < TOLERANCE){position = 0;}// Straight line case

	if(pot1 < TOLERANCE && pot8 >= TOLERANCE){position =-4;}
	if(pot2 < TOLERANCE && pot8 >= TOLERANCE){position =-3;}
	if(pot3 < TOLERANCE && pot8 >= TOLERANCE){position =-2;}
	if(pot4 < TOLERANCE && pot5 >= TOLERANCE){position =-1;}
	if(pot5 < TOLERANCE && pot4 >= TOLERANCE){position = 1;}
	if(pot6 < TOLERANCE && pot1 >= TOLERANCE){position = 2;}
	if(pot7 < TOLERANCE && pot1 >= TOLERANCE){position = 3;}
	if(pot8 < TOLERANCE && pot1 >= TOLERANCE){position = 4;}

	else if(
		pot1 > TOLERANCE &&
		pot2 > TOLERANCE &&
		pot3 > TOLERANCE &&
		pot4 > TOLERANCE &&
		pot5 > TOLERANCE &&
		pot6 > TOLERANCE &&
		pot7 > TOLERANCE &&
		pot8 > TOLERANCE
	){
		OCR0A = 0; 
		OCR0B = 0;
	}

	setMotorSpeeds( BASE - position, BASE + position);

	return position;
}

void PID(double kp, double kd, int base){
	int current_pos = current_position();
	int error = 0 - current_pos;
	int derivative = error - *last_error;
	int control = (kp * error) + (kd * derivative);
	setMotorSpeeds(BASE + control, BASE - control);
	*last_error = error;
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
		current_position();
		PID(Kp, Kd, BASE);
	}
}
