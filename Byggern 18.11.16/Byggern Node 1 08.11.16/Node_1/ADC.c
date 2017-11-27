/*
 * ADC.c
 *
 * Created: 13.09.2016 14:23:25
 *  Author: tobiasu
 */


/*! \file *********************************************************************
 *
 * \brief ADC driver.
 *
 * Initializing ADC. Also function for reading from ADC.
 *
 *
 *
 *****************************************************************************/

#include <avr/io.h>
#include<stdio.h>
#include "ADC.h"
#include "util/delay.h"
#include <avr/interrupt.h>
#define set_bit(reg, bit ) (reg |= (1 << bit));
#define clear_bit(reg, bit ) (reg &= ~(1 << bit));
#define test_bit(reg, bit ) (reg & (1 << bit))

// RAM-memory location:
volatile char *ADC = (char *) 0x1400;



/*! \brief  Initialize ADC.
 *  Set up the ADC for usage, setting pull up for polling
 *
 *  \param void
 *
 *  \return void
 */

void ADC_init(){
    //rising edge generates interupt request:
	set_bit(MCUCR, ISC11);
	set_bit(MCUCR, ISC10);
    
	set_bit(EMCUCR, INT0);
	// set up internal pull-up for PB3 for polling:
	set_bit(PORTB,PB3);
	clear_bit(DDRB,PB3); 

}


/*! \brief  Reading ADC.
 *  choosing channel to read, then wait til convertion is done. 
 *  Returns converted value.
 *
 *  \param uint8_t channel, Which pin ADC should convert
 *
 *  \return uint8_t, Discreteized value
 */

uint8_t ADC_read(uint8_t channel){
	*ADC = 0x3+channel;
    //Polling on ADC ended-convertion:
	while((test_bit(PINB, PB3))){}
	return *ADC;
}





