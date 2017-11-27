/*
 * PWM_TIMER.c
 *
 * Created: 25.10.2016 10:47:56
 *  Author: tobiasu
 */ 

#include "PWM_TIMER.h"
#include <avr/io.h>

#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit ) (reg &= ~(1 << bit))

void pwm_timer_init(){
	
	set_bit(DDRB, PB5); //setter OC1A pin til output
	
	
	set_bit(TCCR1B, WGM13);
	set_bit(TCCR1B,WGM12);
	set_bit(TCCR1A,WGM11); // SET fast PWM-mode 
	clear_bit(TCCR1A,WGM10);
	//
	clear_bit(TCCR1A, COM1A0);
	set_bit(TCCR1A,COM1A1); //Setter non inverted mode...
	//
	set_bit(TCCR1B,CS11); //set prescaler 8
	//
	ICR1 = 40000; //setter periode til 20ms
	OCR1A = 3000; // setter flagg
	
}


void pwm_set_width(int width){
	if ((width < 4200) && (width > 1800)){
		OCR1A = width;
	} 	 
}

