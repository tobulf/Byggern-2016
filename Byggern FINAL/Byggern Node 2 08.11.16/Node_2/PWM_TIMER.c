/*
 * PWM_TIMER.c
 *
 * Created: 25.10.2016 10:47:56
 *  Author: tobiasu
 */ 

/*! \file *********************************************************************
 *
 * \brief PWM-TIMER DRIVER.
 *
 *  Initializes timer for Pulse Width Modulation on servo. 
 *
 *
 *
 *****************************************************************************/


#include "PWM_TIMER.h"
#include <avr/io.h>


// Macros:
#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit ) (reg &= ~(1 << bit))



/*! \brief  timer initialize.
 *  function to set up and initialize timer.
 *
 *  \param void
 *
 *  \return void
 */

void pwm_timer_init(){
	// Set OC1A pin to output:
	set_bit(DDRB, PB5); 
	set_bit(TCCR1B, WGM13);
	set_bit(TCCR1B,WGM12);
	// SET fast PWM-mode:
	set_bit(TCCR1A,WGM11); 
	clear_bit(TCCR1A,WGM10);
    //Setter non-inverted mode:
	clear_bit(TCCR1A, COM1A0);
	set_bit(TCCR1A,COM1A1); 
	//set prescaler 8:
	set_bit(TCCR1B,CS11); 
	//set period to 20ms:
	ICR1 = 40000;
	 // Set flag:
	OCR1A = 3000;
}




/*! \brief  Set Width.
 *  function to set signal width.
 *
 *  \param int width, width of the signal.
 *
 *  \return void
 */

void pwm_set_width(int width){
	// Threshold to keep signal between 0.9 - 2.1 ms:
	if ((width < 4200) && (width > 1800)){
		// Set width:
		OCR1A = width;
	} 	 
}

