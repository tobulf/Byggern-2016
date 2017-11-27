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
#define F_CPU 16000000





	
int main(void)
{	
	
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

	
	//
	//printf("fisk\n");
	//_delay_ms(300);
	CAN_message msg;

    while(1)
    {	
		
		CAN_message_recieve(&msg);
		message_decoder(msg);
        //TODO:: Please write your application code 
    }
}