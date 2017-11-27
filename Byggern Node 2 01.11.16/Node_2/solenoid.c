/*
 * solenoid.c
 *
 * Created: 01.11.2016 17:51:31
 *  Author: bragesae
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include "solenoid.h"
#include "util/delay.h"
#define test_bit(reg, bit ) (reg & (1 << bit))
#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit ) (reg &= ~(1 << bit))


void solenoid_init(){
	set_bit(DDRF,PF1);
}


void trigger_solenoid(){
	set_bit(PORTF,PF1);
	_delay_ms(40);
	clear_bit(PORTF,PF1);
}