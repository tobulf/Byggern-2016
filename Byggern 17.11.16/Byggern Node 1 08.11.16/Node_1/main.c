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
#include "MCP2515.h"
#include <stdio.h>
#include "util/delay.h"
#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit ) (reg &= ~(1 << bit))
#define test_bit(reg, bit ) (reg & (1 << bit))
#include "menu.h"
#include "MCPdriver.h"
#include "CAN.h"


typedef enum {
	MENU,
	GAME
} STATE;

int main(void)
{
	
	USART_init();
	set_bit(MCUCR, SRE);
	set_bit(SFIOR,XMM2);
	set_bit(MCUCR,SRW10);
	set_bit(EMCUCR,SRW11);
	
	
	set_bit(DDRD, 6); // Write pin Atmega, output
	set_bit(DDRD, 7); // Read Pin Atmega, output
	set_bit(DDRE, 1); //Adresse Latch Enable
	clear_bit(DDRB, 6);
	clear_bit(DDRB, 7);
	clear_bit(DDRB, 8);
	ADC_init();
	OLED_init();
	CAN_init();
	joystick_init(); // NB! Must be initialized after CAN:
	MENU_init_menu();
	
	
	DIR joystick_direction = NEUTRAL;
	MENU_print_menu();
	volatile game_settings settings;
	volatile uint8_t startgame = 0;
	STATE state= MENU;
	volatile int score;
	while(1){
		switch(state){
			case MENU:
				joystick_direction = getJoyDir();
				_delay_ms(100);
		
				if (getJoyDir() != joystick_direction){
					_delay_ms(50);
					joystick_direction = getJoyDir();
					MENU_navigate(joystick_direction);
			
					if (joystick_direction == RIGHT){
						MENU_selectAction(&settings, &startgame);
						if (startgame == 1){
							
							CAN_send_game_settings(settings);
							MENU_goto_main_menu();
							OLED_reset();
							OLED_print_pig();
							state = GAME;
						}
				
					}
				}
				break;
				
			case GAME:
				joystick_direction = getJoyDir();
				IO_send_CAN(joystick_direction);
				CAN_message msg;
				CAN_message_recieve(&msg);
				
				if (msg.id == SENDSCORE){
					GAME_print_score(msg);

				}
				if (msg.id == GAMEOVER){
					GAME_print_finalscore(msg);
					while (!leftButtonPressed() && !rightButtonPressed() && !joyStickButtonPressed()){}
					//_delay_ms(2000);
					MENU_goto_main_menu();
					startgame = 0;
					state = MENU;
				}
				break;
		}
		
		/*
		
 		MCP2515_write(0x0F, 0x54);
		char result = MCP2515_read(0x0F);
 		printf("%d\n",result);
 		_delay_ms(400);
 		
		joystick_direction = getJoyDir();
		IO_send_CAN(joystick_direction);
		_delay_ms(100);
		
 		if (getJoyDir() != joystick_direction){
			_delay_ms(50); 
 			joystick_direction = getJoyDir();
			MENU_navigate(joystick_direction);
			
			if (joystick_direction == RIGHT){
				MENU_selectAction(&startgame_arr);
				//MCP2515_requestToSend(0);
				if (startgame_arr[1] != 0){
					
				}
				
			}
 		}
 
		 
		 
		 
		 
 	*/
	}
}
