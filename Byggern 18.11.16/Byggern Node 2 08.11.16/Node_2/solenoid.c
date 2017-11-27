/*
 * solenoid.c
 *
 * Created: 01.11.2016 17:51:31
 *  Author: bragesae
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include "CAN.h"
#include "solenoid.h"
#include "util/delay.h"
#define test_bit(reg, bit ) (reg & (1 << bit))
#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit ) (reg &= ~(1 << bit))
volatile uint8_t SOLENOID_MODE = SOLENOID_SEMI_AUTO;
volatile uint8_t SOLENOID_BUTTON_PRESSED = 0; //solenoid triggermode = semiautomatic
volatile uint8_t SOLENOID_COUNTER = 0; //solenoid triggermode = automatic
void solenoid_init(){
	set_bit(DDRF,PF1);
}

void set_solenoid_mode(CAN_message msg){
	if (msg.data[2] == SOLENOID_AUTO){
		SOLENOID_MODE = SOLENOID_AUTO;
	}
	if (msg.data[2] == SOLENOID_SEMI){
		SOLENOID_MODE = SOLENOID_SEMI;
	}
	if (msg.data[2] == SOLENOID_SEMI_AUTO){
		SOLENOID_MODE = SOLENOID_SEMI_AUTO;
	}
}


void trigger_solenoid(){
	set_bit(PORTF,PF1);
	_delay_ms(35);
	clear_bit(PORTF,PF1);
	_delay_ms(5);
}


void trigger_solenoid_mode(CAN_message msg){
	if (SOLENOID_MODE == SOLENOID_SEMI){
		if ((msg.data[5] == 1)  && (SOLENOID_BUTTON_PRESSED == 0)){
			trigger_solenoid();
			SOLENOID_BUTTON_PRESSED = 1;
		}
		else if ((msg.data[5] == 0)  && (SOLENOID_BUTTON_PRESSED == 1)){
			SOLENOID_BUTTON_PRESSED = 0;
		}
	}
	if (SOLENOID_MODE == SOLENOID_SEMI_AUTO){
		SOLENOID_COUNTER++;
		if (msg.data[5] == 1){
			if (SOLENOID_COUNTER > 2){
				trigger_solenoid();
				SOLENOID_COUNTER = 0;
			}
		
			
		}
	}
	if (SOLENOID_MODE == SOLENOID_AUTO){
		SOLENOID_COUNTER++;
		if (SOLENOID_COUNTER > 5){
			SOLENOID_COUNTER = 0;
			trigger_solenoid();
		}
	}
}