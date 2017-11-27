/*
 * timer.c
 *
 * Created: 25.10.2016 13:55:09
 *  Author: tobiasu
 */ 

/*! \file *********************************************************************
 *
 * \brief TIMER DRIVER.
 *
 *  Initializes timer for interrupts.
 *
 *
 *****************************************************************************/



#include <stddef.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "timer.h"

// Macros:
#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit ) (reg &= ~(1 << bit))

// Interrupt-flags:
volatile uint8_t* flagPtr_ = NULL;
volatile uint8_t* PI_flagPtr_ = NULL;

// Variables for timing:
volatile int time = 0;
volatile int time_count = 0;




/*! \brief  Timer init
 *  Initializes interrupt timer.
 *
 *  \param uint8_t* flagptr, Interrupt flag.
 *
 *  \return void
 */

void timer_init(uint8_t* flagPtr){	
	 // SET CTC-mode:
	set_bit(TCCR3B, WGM32); 
	//set prescaler 8:
	set_bit(TCCR3B,CS11); 
	uint8_t ms = 20;
	int value = 16000/8;
	value *= ms;
	value -= 1;
	OCR3A = value;
	flagPtr_ = flagPtr;
	//interrupt:
	set_bit(TIMSK3, OCIE3A);

	sei();
}


ISR(TIMER3_COMPA_vect){
	if(flagPtr_ != NULL)
		*flagPtr_ = 1;
 	clear_bit(TIFR3, OCF3A);
	
}



/*! \brief  score Timer init
 *  Initializes score timer.
 *
 *  \param void
 *
 *  \return void
 */

void score_timer(){
	// SET CTC-mode:
	set_bit(TCCR4B, WGM32); 

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



	
/*! \brief  Timer init
 *  Initializes interrupt timer.
 *
 *  \param uint8_t* flagptr, Interrupt flag.
 *
 *  \return void
 */
int score_get_time(){
	return time;
}




/*! \brief  get score.
 *  Function for geting score.
 *
 *  \param void
 *
 *  \return int score time, returns score.
 */

int score_get_score(){
	return score_get_time();
}




/*! \brief  score reset
 *  reset score-timer.
 *
 *  \param void
 *
 *  \return void
 */

void score_reset_time(){
	time = 0;
}




ISR(TIMER4_COMPA_vect){
	time_count++;
	if (time_count > 9){
		time_count = 0;
		time++;
	}
	clear_bit(TIFR4, OCF4A);
}



/*! \brief  PI Timer init
 *  Initializes PI-timer.
 *
 *  \param uint8_t* flagptr, Interrupt flag.
 *
 *  \return void
 */

void PI_timer(uint8_t *PI_flagPtr){
	// SET CTC-mode:
	set_bit(TCCR5B, WGM32);  
	//set prescaler 8:
	set_bit(TCCR5B,CS11); 
	uint8_t ms = 10;
	int value = 16000/8;
	value *= ms;
	value -= 1;
	OCR4A = value;
	PI_flagPtr_ = PI_flagPtr;
	//interrupt:
	set_bit(TIMSK5, OCIE5A);

	sei();
}

ISR(TIMER5_COMPA_vect){
	if(PI_flagPtr_ != NULL){
		*PI_flagPtr_ = 1;
	}

	clear_bit(TIFR5, OCF5A);
}