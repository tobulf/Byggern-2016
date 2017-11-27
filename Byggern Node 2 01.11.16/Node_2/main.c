/*
 * Node_2.c
 *
 * Created: 18.10.2016 10:22:46
 *  Author: tobiasu
 */ ;
#include <avr/io.h>
#include "UARTdriver.h"
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include "util/delay.h"
#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit ) (reg &= ~(1 << bit))
#define test_bit(reg, bit ) (reg & (1 << bit))
#include "MCPdriver.h"
#include "CAN.h"
#include "ADC.h"
#define F_CPU 16000000
#include "solenoid.h"

#include "timer.h"




	
int main(void)
{	

	
	volatile uint8_t timerFlag = 0;
	volatile uint8_t ADCFlag = 0;
	volatile int time;
	volatile uint8_t IR_vector[4]; 
	volatile uint8_t score = 0;
	//set_bit(MCUCR, SRE);
	//
	//set_bit(MCUCR,SRW10);
	//
	//
	////SRAM_test();
	//set_bit(DDRD, 6); // Write pin Atmega, output
	//set_bit(DDRD, 7); // Read Pin Atmega, output
	//set_bit(DDRE, 1); //Adresse Latch Enable
	//clear_bit(DDRB, 6);
	//clear_bit(DDRB, 7);
	//clear_bit(DDRB, 8);
	USART_init();
	CAN_init();
	ADC_init(&ADCFlag);
	CAN_message msg;
	pwm_timer_init();
	timer_init(&timerFlag);
	score_timer();
	score_reset_time();
	solenoid_init();
    while(1)
    {	
		clear_bit(PORTD, PD3);		
		if (timerFlag){
			CAN_message_recieve(&msg);
			message_decoder(msg);
			timerFlag = 0;
		}
		
		if (ADCFlag){
			fill_IR_vector(&IR_vector);
			if (IR_blocked(IR_vector)){
				printf("final score: %d \n", score_get_time());
				score_reset_time();
			}
		}

		
		_delay_ms(200);

		
        //TODO:: Please write your application code 
    }
}




