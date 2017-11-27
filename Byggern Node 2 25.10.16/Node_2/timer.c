/*
 * timer.c
 *
 * Created: 25.10.2016 13:55:09
 *  Author: tobiasu
 */ 

#include <stddef.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit ) (reg &= ~(1 << bit))

uint8_t* flagPtr_ = NULL;

void timer_init(uint8_t* flagPtr)
{
	set_bit(TCCR3B, WGM32);  // SET CTC-mode
	//set_bit(TCCR3B,COM1A1); //Setter non inverted mode...
	set_bit(TCCR3B,CS11); //set prescaler 8
	//
	//ICR3 = 40000; //setter periode til 20ms

	uint8_t ms = 20;
	int value = 16000/8;
	value *= ms;
	value -= 1;
	OCR3A = value;
	
	flagPtr_ = flagPtr;
	
	//interrupt
	set_bit(TIMSK3, OCIE3A);

	sei();
}


ISR(TIMER3_COMPA_vect){
	if(flagPtr_ != NULL)
		*flagPtr_ = 1;

	
 	clear_bit(TIFR3, OCF3A);
	
}