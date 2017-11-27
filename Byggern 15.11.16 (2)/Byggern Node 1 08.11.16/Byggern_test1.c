/*
 * Byggern_test1.c
 *
 * Created: 30.08.2016 14:02:48
 *  Author: tobiasu
 */ 


#include "UARTdriver.h"
#include "SRAM_test.h"
#include <avr/interrupt.h>
#include "ADC.h"

#define F_CPU 4915200
#include <avr/io.h>
#include <stdio.h>
#include "util/delay.h"
#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit ) (reg &= ~(1 << bit))
#define test_bit(reg, bit ) (reg & (1 << bit))






int main(void)
{
	USART_init();
	set_bit(MCUCR, SRE);
	set_bit(SFIOR,XMM2);
	set_bit(MCUCR,SRW10);
	
	set_bit(EMCUCR,SRW11);
	SRAM_test();
	set_bit(DDRD, 6); // Write pin Atmega, output
	set_bit(DDRD, 7); // Read Pin Atmega, output
	set_bit(DDRE, 1); //Adresse Latch Enable
	clear_bit(DDRB, 3);
	clear_bit(DDRB, 4);
	volatile char *ADC = (char *) 0x1400;
	sei();
	ADC_init();
	uint8_t dataX;
    uint8_t dataY;
	int data_modified = 0;
	while(1)
    {
		//ISR(INT0_vect);
		//variabel = ADC[0];
		//PORTC |= (1 << PC0);
		//set_bit(PORTC, 0);
		//set_bit(PORTC, 2);
		//clear_bit(PORTC, 1);
		//clear_bit(PORTC, 2);
		//clear_bit(PORTC, 3);
		//clear_bit(PORTC, 2);
		//set_bit(PORTE, 1);
		//clear_bit(PORTE, 1);
		//clear_bit(PORTA, 0);
		//clear_bit(PORTA, 1);
		/*set_bit(PORTA, 0);
		_delay_ms(500);
		clear_bit(PORTA, 0);
		_delay_ms(500);
        //TODO:: Please write your application code 
		set
		*/
		dataX = ADC_read(1);
		///_delay_us(60);
		//dataY = ADC_read(2);
	
		
		if (adcflag ==1 ){
			printf("test");
		}
		
		//data_modified = dataX*(200)/(255)-100;
		//printf("x-akse: %d        , y-akse: %d\n", dataX, dataY);
		//printf("%d\n", dataX);
		/*if(test_bit(PINB, PB2 ) ){
			printf("Right button\n");	
		}
		if(test_bit(PINB, PB3 ) ){
			printf("Left button\n");
		}*/
		_delay_ms(100);
	}
		//char rec = USART_receive();
		//USART_transmit(rec);
	
		printf("Hello World. fesk \n");
	
	}