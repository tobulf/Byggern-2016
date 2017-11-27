/*
 * Byggern_test1.c
 *
 * Created: 30.08.2016 14:02:48
 *  Author: tobiasu
 */ 


#include "UARTdriver.h"
#include "SRAM_test.h"
#include <avr/interrupt.h>
#include <stdbool.h>
#include "joystick.h"
#include "ADC.h"
#include "OLEDdriver.h"
#include <stdbool.h>
#define F_CPU 4915200
#include <avr/io.h>
#include <stdio.h>
#include "util/delay.h"
#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit ) (reg &= ~(1 << bit))
#define test_bit(reg, bit ) (reg & (1 << bit))
#include "menu.h"
#include "MCPdriver.h"

int main(void)
{
	
	USART_init();
	set_bit(MCUCR, SRE);
	set_bit(SFIOR,XMM2);
	set_bit(MCUCR,SRW10);
	set_bit(EMCUCR,SRW11);
	
	//SRAM_test();
	set_bit(DDRD, 6); // Write pin Atmega, output
	set_bit(DDRD, 7); // Read Pin Atmega, output
	set_bit(DDRE, 1); //Adresse Latch Enable
	clear_bit(DDRB, 6);
	clear_bit(DDRB, 7);
	clear_bit(DDRB, 8);
	ADC_init();
	joystick_init();
	OLED_init();
	SPI_init();
	
	
	
	
	MENU_init_menu();
	DIR joystick_direction = NEUTRAL;
	MENU_print_menu();

	while(1){
 		
 		
 		if (getJoyDir() != joystick_direction){
			_delay_ms(50); 
 			joystick_direction = getJoyDir();
			MENU_navigate(joystick_direction);
			if (joystick_direction == RIGHT){
				MENU_selectAction();
					
			}
			
			
				
 		}
		 
		
 		
 		//OLED_reset();
 		

		

		

	
	}

	
}