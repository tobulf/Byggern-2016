/*
 * timer.c
 *
 * Created: 25.10.2016 13:55:09
 *  Author: tobiasu
 */ 

#include <stddef.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "timer.h"


#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit ) (reg &= ~(1 << bit))

volatile uint8_t* flagPtr_ = NULL;
volatile uint8_t* PI_flagPtr_ = NULL;

volatile int time = 0;

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


/*
double get_wall_time(void){
	struct timeval time;
	gettimeofday(&time, 0);
	return (double)time.tv_sec + (double)time.tv_usec * .000001);
}

void timer_start(){
	starttime = get_wall_time();
}


*/

void score_timer(){
	
	set_bit(TCCR4B, WGM32);  // SET CTC-mode
	//set_bit(TCCR3B,COM1A1); //Setter non inverted mode...
	set_bit(TCCR4B,CS11); //set prescaler 8
	//
	//ICR3 = 40000; //setter periode til 20ms

	uint8_t ms = 100;
	int value = 16000/8;
	value *= ms;
	value -= 1;
	OCR4A = value;
	//interrupt
	set_bit(TIMSK4, OCIE4A);

	sei();
}	

int score_get_time(){
	return time;
}

void score_reset_time(){
	time = 0;
}

ISR(TIMER4_COMPA_vect){
	time++;
	clear_bit(TIFR4, OCF4A);
}




void PI_timer(uint8_t *PI_flagPtr){
	
	set_bit(TCCR5B, WGM32);  // SET CTC-mode
	//set_bit(TCCR3B,COM1A1); //Setter non inverted mode...
	set_bit(TCCR5B,CS11); //set prescaler 8
	//
	//ICR3 = 40000; //setter periode til 20ms

	uint8_t ms = 10;
	int value = 16000/8;
	value *= ms;
	value -= 1;
	OCR4A = value;
	PI_flagPtr_ = PI_flagPtr;
	//interrupt
	set_bit(TIMSK5, OCIE5A);

	sei();
}

ISR(TIMER5_COMPA_vect){
	if(PI_flagPtr_ != NULL){
		*PI_flagPtr_ = 1;
	}

	clear_bit(TIFR5, OCF5A);
}