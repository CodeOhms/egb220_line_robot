#define F_CPU 16000000UL //16 MHz
#include <avr/io.h>
//#include <avr/delay.h>
//#include <avr/interrupt.h>
//#include <stdlib.h>

#define SET_BIT(reg, pin)        (reg) |= (1 << (pin))
#define CLEAR_BIT(reg, pin)     (reg) &= ~(1 << (pin))

void ADC_init();



void ADC_init()
{
  DDRB |= (1 << 4);
  //ADMUX = 0b01000000; 
  ADCSRA |= (1<<ADEN)|( 1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}


int main()
{
  ADC_init();
  
  while(1)
  {
   // Start single conversion by setting ADSC bit in ADCSRA
 

 ADMUX = 0b01000000;
   ADCSRA |= (1 << ADSC);
  while(~ADCSRA&(1<<ADIF)){}
  // Result now available.
   uint16_t pot2 = ADC;
   
    if (pot2 > 1000)
     // PORTB |= (1<<4);       // output on port b - pin 0
      SET_BIT(PORTB, 0);
    else
   //   PORTB &= ~(1<<4);       // output on port b - pin 0
      CLEAR_BIT(PORTB, 0);

ADMUX = 0b01000001;
   ADCSRA |= (1 << ADSC);
  while(~ADCSRA&(1<<ADIF)){}
  // Result now available.
   uint16_t pot3 = ADC;
   
    if (pot3 > 1000)
     // PORTB |= (1<<4);       // output on port b - pin 0
      SET_BIT(PORTB, 1);
    else
   //   PORTB &= ~(1<<4);       // output on port b - pin 0
      CLEAR_BIT(PORTB, 1);

  }
}

//https://ww1.microchip.com/downloads/en/devicedoc/atmel-7766-8-bit-avr-atmega16u4-32u4_datasheet.pdf
