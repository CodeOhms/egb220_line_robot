#define F_CPU 16000000UL //16 MHz
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define toggle(A, B) A ^= (1<<B)
#define set(A, B) A |= (1<<B)
#define clr(A, B) A &= ~(1<<B)

// 24.9.1
#define SENSOR1 (0<<MUX5)|(0<<MUX4)|(0<<MUX3)|(1<<MUX2)|(0<<MUX1)|(0<<MUX0)
#define SENSOR2 (0<<MUX5)|(0<<MUX4)|(0<<MUX3)|(1<<MUX2)|(0<<MUX1)|(1<<MUX0)
#define SENSOR3 (0<<MUX5)|(0<<MUX4)|(0<<MUX3)|(1<<MUX2)|(1<<MUX1)|(0<<MUX0)
#define SENSOR4 (0<<MUX5)|(0<<MUX4)|(0<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0)
#define SENSOR5 (0<<MUX5)|(0<<MUX4)|(0<<MUX3)|(0<<MUX2)|(1<<MUX1)|(1<<MUX0)
#define SENSOR6 (1<<MUX5)|(0<<MUX4)|(0<<MUX3)|(0<<MUX2)|(1<<MUX1)|(0<<MUX0)
#define SENSOR7 (1<<MUX5)|(0<<MUX4)|(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(1<<MUX0)
#define SENSOR8 (1<<MUX5)|(0<<MUX4)|(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(0<<MUX0)
            // SENSOR1 = PF4 = ADC4
            // SENSOR2 = PF5 = ADC5
            // SENSOR3 = PF6 = ADC6
            // SENSOR4 = PF7 = ADC7
            // SENSOR5 = PB4 = ADC11
            // SENSOR6 = PD7 = ADC10
            // SENSOR7 = PD6 = ADC9
            // SENSOR8 = PD4 = ADC8

//int motor_init() {
//	//Motor 2
//	DDRB |= (1<<0); // Set Bit/Pin 0 to 1/Output
//	DDRB |= (1<<7); // Set Bit/Pin 0 to 1/Output
//	//Motor 1
//	DDRE |= (1<<6); // Set Bit/Pin 0 to 1/Output
//	DDRD |= (1<<0); // Set Bit/Pin 0 to 1/Output	
//	
//	//Timers - 13.8
//	TCCR0A |= (1<<7)|(1<<5)|(1<<1)|(1<<0);
//	TCCR0B |= (1<<2);
//	TIMSK0 |= (1<<0);
//	OCR0A = 20;	
//	OCR0B = 1;	
//
//	return 0;
//}

void ADC_init()
{
	ADMUX |= (1<<6)|(1<<5);
	ADCSRA |= (1<<7)|(1<<5)|(1<<3)|(1<<2)|(1<<1)|1;
	// enable adc, enable auto-trigger, enable interrupt & interrupt flag, clock prescaler of 128, start conversion

	ADCSRB = 0; 
	// free running mode

	ADCSRA |= (1<<6);
	// on
	USBCON = 0;
	sei();
}

// ADC Interrupt
void sensor_data(){
	for(int i = 0; i < 8; i++){
	ADMUX |= (1<<4)|(1<<3)|(1<<2)|(1<<1)|(1<<0)

    if (i == 0) {ADMUX |= SENSOR1; ADCSRB |= (0<<MUX5);}
    if (i == 1) {ADMUX |= SENSOR2; ADCSRB |= (0<<MUX5);}
    if (i == 2) {ADMUX |= SENSOR3; ADCSRB |= (0<<MUX5);}
    if (i == 3) {ADMUX |= SENSOR4; ADCSRB |= (0<<MUX5);}
    if (i == 4) {ADMUX |= SENSOR5; ADCSRB |= (0<<MUX5);}
    if (i == 5) {ADMUX |= SENSOR6; ADCSRB |= (0<<MUX5);}
    if (i == 6) {ADMUX |= SENSOR7; ADCSRB |= (0<<MUX5);}
    if (i == 7) {ADMUX |= SENSOR8; ADCSRB |= (0<<MUX5);}

	ADCSRA |= (1<<ADSC);
	while(~ADCSRA&(1<<ADIF)){}
	}
}

int main()
	ADC_init();
	while(1){
		sensor_data();
	
		if(ADMUX & 4 || ADMUX & 5 || ADMUX & 6 || ADMUX & 7 || ADMUX & 11 || ADMUX & 10 || ADMUX & 9 || ADMUX & 8){
			DDRB |= (1<<0);
			PORTB = (1<<0);
	}
		else{
			DDRB |= (1<<2);        // initialize port B
			PORTB = (1<<2);       // output on port b - pin 0
		}
	}
}
