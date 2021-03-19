#define F_CPU 16000000UL //16 MHz
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define SENSOR8 (0<<MUX4)|(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(0<<MUX0)
#define SENSOR7 (0<<MUX4)|(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(1<<MUX0)
#define SENSOR6 (0<<MUX4)|(0<<MUX3)|(0<<MUX2)|(1<<MUX1)|(0<<MUX0)
#define SENSOR5 (0<<MUX4)|(0<<MUX3)|(0<<MUX2)|(1<<MUX1)|(1<<MUX0)
#define SENSOR4 (0<<MUX4)|(0<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0)
#define SENSOR3 (0<<MUX4)|(0<<MUX3)|(1<<MUX2)|(1<<MUX1)|(0<<MUX0)
#define SENSOR2 (0<<MUX4)|(0<<MUX3)|(1<<MUX2)|(0<<MUX1)|(1<<MUX0)
#define SENSOR1 (0<<MUX4)|(0<<MUX3)|(1<<MUX2)|(0<<MUX1)|(0<<MUX0)
            // SENSOR1 = PF4
            // SENSOR2 = PF5
            // SENSOR3 = PF6
            // SENSOR4 = PF7
            // SENSOR5 = PB4
            // SENSOR6 = PD7
            // SENSOR7 = PD6
            // SENSOR8 = PD4
            
uint8_t sensor_out[8]; 

void ADC_init();
void sensor_data();


void ADC_init()
{
  ADMUX |= (1<<REFS1)|(1<<REFS0)|(1<<ADLAR); 
  ADCSRA |= (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
  ADCSRB = 0; 
}

void sensor_data()
{ 
  int i;
  for (i = 0; i < 8; i++)
  {
    ADMUX |= (0<<MUX4)|(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(0<<MUX0);
    
    if (i == 0) {ADMUX |= SENSOR1; ADCSRB |= (0<<MUX5);}
    if (i == 1) {ADMUX |= SENSOR2; ADCSRB |= (0<<MUX5);}
    if (i == 2) {ADMUX |= SENSOR3; ADCSRB |= (0<<MUX5);}
    if (i == 3) {ADMUX |= SENSOR4; ADCSRB |= (0<<MUX5);}
    if (i == 4) {ADMUX |= SENSOR5; ADCSRB |= (0<<MUX5);}
    if (i == 5) {ADMUX |= SENSOR6; ADCSRB |= (0<<MUX5);}
    if (i == 6) {ADMUX |= SENSOR7; ADCSRB |= (0<<MUX5);}
    if (i == 7) {ADMUX |= SENSOR8; ADCSRB |= (0<<MUX5);}
    
    ADCSRA |= (1<<ADSC); //conversion
    while(~ADCSRA&(1<<ADIF)){}
    sensor_out[i] = ADCH;
  }
}

int main()
{
  ADC_init();
  
  while(1)
  {
    sensor_data();
    
    if (sensor_out[7] < 30)
    {
      DDRB |= (1<<0);        // initialize port B
      PORTB = (1<<0);       // output on port b - pin 0
    }
    else
    {
      DDRB |= (1<<2);        // initialize port B
      PORTB = (1<<2);       // output on port b - pin 0
    }
  }
}

//https://ww1.microchip.com/downloads/en/devicedoc/atmel-7766-8-bit-avr-atmega16u4-32u4_datasheet.pdf
