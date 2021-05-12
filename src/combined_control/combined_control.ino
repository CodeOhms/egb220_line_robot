#define F_CPU 16000000UL //16 MHz
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define TOLERANCE 180

uint16_t pot8;
uint16_t pot7;
uint16_t pot6;
uint16_t pot5;
uint16_t pot4;
uint16_t pot3;
uint16_t pot2;
uint16_t pot1;

int Base;
int BangBang;
int error;
int position = 0;
int *last_error = 0;

double Kp;
double Kd = 0.95;

void pwm_init(){
  TCCR0A |= (1<<WGM00);
  TCCR0A |= (1<<WGM01);
  TCCR0B &= ~(1<<WGM02);

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


void adc_init(){
  ADMUX |= (1<<REFS1)|(1<<REFS0)|(1<<ADLAR); //2.56V ref, left adjusted

  // enable adc, clock prescaler of 128
  ADCSRA |= (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1);
  ADCSRB = 0;
}

void sen_8() {
  ADMUX = 0b11100000;
  ADCSRB = 0b00100000;    // enable adc 8 -> 100000
  ADCSRA |= (1<<6);     // start conversion
  while(ADCSRA&(1<<ADSC)){}   // wait for conversion to complete
  pot8 = ADCH;
}

void sen_7() {
  ADMUX = 0b11100001; // sensor 7 -> adc9
  ADCSRB = 0b00100000;
  ADCSRA |= (1<<6);
  while(ADCSRA&(1<<ADSC)){}
  pot7 = ADCH;
}

void sen_6() {
  ADMUX = 0b11100010; // sensor 6 -> adc10
  ADCSRB = 0b00100000;
  ADCSRA |= (1<<6);
  while(ADCSRA&(1<<ADSC)){}
  pot6 = ADCH;
}

void sen_5() {
  ADMUX = 0b11100011; // sensor 5 -> adc11
  ADCSRB = 0b00100000;
  ADCSRA |= (1<<6);
  while(ADCSRA&(1<<ADSC)){}
  pot5 = ADCH;
}

void sen_4() { 
  ADMUX = 0b11100111; // sensor 4 -> adc7
  ADCSRB = 0b00000000;
  ADCSRA |= (1<<6);
  while(ADCSRA&(1<<ADSC)){}
  pot4 = ADCH;
}

void sen_3() {  // sensor 3 -> adc6
  ADMUX = 0b11100110;
  ADCSRB = 0b00000000;
  ADCSRA |= (1<<6);
  while(ADCSRA&(1<<ADSC)){}
  pot3 = ADCH;
}

void sen_2() { // sensor 2 -> adc5
  ADMUX = 0b11100101;
  ADCSRB = 0b00000000;
  ADCSRA |= (1<<6);
  while(ADCSRA&(1<<ADSC)){}
  pot2 = ADCH;
}

void sen_1() {  // sensor 1 -> adc4
  ADMUX = 0b11100100;
  ADCSRB = 0b00000000;
  ADCSRA |= (1<<6);
  while(ADCSRA&(1<<ADSC)){}
  pot1 = ADCH;
}

void setMotorSpeeds(double motorA, double motorB) {
  OCR0A = 255 * motorA/100; 
  OCR0B = 255 * motorB/100;
}

int current_position(){
	if(pot1 < TOLERANCE && pot2 < TOLERANCE  && pot3 < TOLERANCE  && pot4 < TOLERANCE
	&& pot5 < TOLERANCE  && pot6 < TOLERANCE  && pot7 < TOLERANCE  && pot8 < TOLERANCE)
	{position = 0;} // Straight line case
  
	if(pot4 < TOLERANCE && pot5 < TOLERANCE){position = 0;}// Straight line case

	if(pot2 < TOLERANCE && pot3 >= TOLERANCE && pot4 >= TOLERANCE && pot5 >= TOLERANCE && pot6 >= TOLERANCE && pot7 >= TOLERANCE){position =-5;}
	if(pot2 < TOLERANCE && pot3 <  TOLERANCE && pot4 >= TOLERANCE && pot5 >= TOLERANCE && pot6 >= TOLERANCE && pot7 >= TOLERANCE){position =-4;}
	if(pot3 < TOLERANCE && pot2 >= TOLERANCE && pot4 >= TOLERANCE && pot5 >= TOLERANCE && pot6 >= TOLERANCE && pot7 >= TOLERANCE){position =-3;}
	if(pot3 < TOLERANCE && pot4 <  TOLERANCE && pot2 >= TOLERANCE && pot5 >= TOLERANCE && pot6 >= TOLERANCE && pot7 >= TOLERANCE){position =-2;}
	if(pot4 < TOLERANCE && pot2 >= TOLERANCE && pot3 >= TOLERANCE && pot5 >= TOLERANCE && pot6 >= TOLERANCE && pot7 >= TOLERANCE){position =-1;}
	if(pot5 < TOLERANCE && pot2 >= TOLERANCE && pot3 >= TOLERANCE && pot4 >= TOLERANCE && pot6 >= TOLERANCE && pot7 >= TOLERANCE){position = 1;}
	if(pot5 < TOLERANCE && pot6 <  TOLERANCE && pot2 >= TOLERANCE && pot3 >= TOLERANCE && pot4 >= TOLERANCE && pot7 >= TOLERANCE){position = 2;}
	if(pot6 < TOLERANCE && pot2 >= TOLERANCE && pot3 >= TOLERANCE && pot4 >= TOLERANCE && pot5 >= TOLERANCE && pot7 >= TOLERANCE){position = 3;}
	if(pot6 < TOLERANCE && pot7 <  TOLERANCE && pot2 >= TOLERANCE && pot3 >= TOLERANCE && pot4 >= TOLERANCE && pot5 >= TOLERANCE){position = 4;}
	if(pot7 < TOLERANCE && pot2 >= TOLERANCE && pot3 >= TOLERANCE && pot4 >= TOLERANCE && pot5 >= TOLERANCE && pot6 >= TOLERANCE){position = 5;}


  //setMotorSpeeds( Base - position, Base + position);
  

  return position;
}

void control(double kp, double kd, int *last_error, int base, int type){
  int current_pos = current_position();
	if(
		pot7 > TOLERANCE
		&& pot6 > TOLERANCE 
		&& pot5 > TOLERANCE
		&& pot4 > TOLERANCE 
		&& pot3 > TOLERANCE
		&& pot2 > TOLERANCE 
		){
		error = *last_error;
//		OCR0A = 0;
//		OCR0B = 0;	
//		delay(1000);
		}
	if(type == 1){
		if(pot4 < TOLERANCE && pot5 < TOLERANCE) { 	// if 4 & 5 see line, go straight
			setMotorSpeeds(85, 85);
		}		
		else if(pot4 >= TOLERANCE && pot5 < TOLERANCE){	// if 5 sees the line and 4 does not SLIGHT LEFT
			setMotorSpeeds(85, 84);
		}
		else if(pot4 < TOLERANCE && pot5 >= TOLERANCE){	// if 4 sees line & 5 does not SLIGHT RIGHT
			setMotorSpeeds(84, 85);
		}
	}
	else{
		error = 0 - current_pos;
		int derivative = error - *last_error;
		int control = (kp * error) + (kd * derivative);
		setMotorSpeeds(base + control, base - control);
		*last_error = error;
	}
}

void read_sensors(){
    sen_7(); 
    sen_6(); 
    sen_5(); 
    sen_4(); 
    sen_3(); 
    sen_2(); 
}
int main(){
	read_sensors();
	adc_init();
	pwm_init();

	while(1) {
		current_position();
		if(position == -1 || position == 0 || position == 1)
		{
			BangBang = 1;
		}

		else{BangBang = 0;
			Base = 55;
			Kp = 5.5;}

    control(Kp, Kd, *last_error, Base, BangBang);
	}
}