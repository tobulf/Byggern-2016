/*
 * ADC.c
 *
 * Created: 13.09.2016 14:23:25
 *  Author: tobiasu
 */ 
#include <avr/io.h>
#include<stdio.h>
#include "ADC.h"
#include "util/delay.h"
#include <avr/interrupt.h>
#define set_bit(reg, bit ) (reg |= (1 << bit));
#define clear_bit(reg, bit ) (reg &= ~(1 << bit));
#define test_bit(reg, bit ) (reg & (1 << bit))

volatile char *ADC = (char *) 0x1400;
int dataX;
int dataY;
int leftSlider;
int rightSlider;

void ADC_init(){
	set_bit(MCUCR, ISC11); //rising edge generates interupt request
	set_bit(MCUCR, ISC10); //rising edge generates interupt request
	set_bit(EMCUCR, INT0); //trenger vi denne?
	//set_bit(GIFR, INTF0); //er det dette flagget vi bruker i ISR(INT0_vect)?
	adcflag = 0;
	
	// set up internal pull-up for PB3
	set_bit(PORTB,PB3);
	clear_bit(DDRB,PB3); 

}


uint8_t ADC_read(uint8_t channel){
	*ADC = 0x3+channel;
	while((test_bit(PINB, PB3))){}
	//_delay_ms(2);
	return *ADC;
}



ISR(INT0_vect){
	//hvordan fungerer denne funksjonen?
	//print data fra slider/joystick?
	
	
	adcflag = 1;
}




