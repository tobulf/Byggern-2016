/*
 * ADC.c
 *
 * Created: 25.10.2016 18:52:26
 *  Author: tobiasu
 */ 


#include "ADC.h"
#include <avr/io.h>

#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit ) (reg &= ~(1 << bit))



void ADC_init(){
	set_bit(ADCSRA, ADEN); //Enable ADC
	
	set_bit(ADMUX, REFS0); // Setter voltage reference til AVCC
	
	set_bit(ADMUX, MUX0);
	set_bit(ADMUX, MUX1); // Setter input til ADC3
		
	set_bit(ADCSRA, ADSC); // Single conversion mode
	
	
	
	
	
}
