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
#include <avr/io.h>
#include "MCP2515.h"
#include <stdio.h>
#include "util/delay.h"
#include "menu.h"
#include "MCPdriver.h"
#include "CAN.h"
#include "EEPROM.h"

// Defining clock frequency:
#define F_CPU 4915200

// Defining macros:
#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit ) (reg &= ~(1 << bit))
#define test_bit(reg, bit ) (reg & (1 << bit))

// DEfining ENUMS:
typedef enum {
	MENU,
	GAME,
    INIT
} STATE;

int main(void)
{
    // Set initial state:
    STATE state = INIT;
	while(1){
		switch(state){
                
            case INIT:
                USART_init();
                // External SRAM enable:
                set_bit(MCUCR, SRE);
                // Release port PC4-PC7 as normal pins:
                set_bit(SFIOR,XMM2);
                // Set external memory timing:
                set_bit(MCUCR,SRW10);
                set_bit(EMCUCR,SRW11);
                
                // Write pin Atmega, output:
                set_bit(DDRD, PD6);
                // Read Pin Atmega, output:
                set_bit(DDRD, PD7);
                // Address Latch Enable:
                set_bit(DDRE, PE1);
                
                // Set MISO/MOSI/SCK as output for CAN:
                clear_bit(DDRB, PB6);
                clear_bit(DDRB, PB7);
                clear_bit(DDRB, PB8);
                
                //initializing:
                ADC_init();
                OLED_init();
                CAN_init();
                joystick_init();
                MENU_init_menu();
                
                // Setting joystick direction to NEUTRAL during initialization:
                DIR joystick_direction = NEUTRAL;
                
                // Printing main manu:
                MENU_print_menu();
                
                // Declaring variables:
                volatile game_settings settings;
                volatile uint8_t startgame = 0;
                volatile int score;
                
                
                // Setting state to MENU:
                STATE state= MENU;
                break;
                
			case MENU:
                
                // Set current joystick direction:
				joystick_direction = getJoyDir();
				_delay_ms(100); // SJEKKE OM DENNE TRENGS!
		
				if (getJoyDir() != joystick_direction){
					_delay_ms(50);
                    
                    //Update current joystick direction:
					joystick_direction = getJoyDir();
                    // Navigate the menu:
					MENU_navigate(joystick_direction);
			
					if (joystick_direction == RIGHT){
                        // Check if the current menu have any actions:
						MENU_selectAction(&settings, &startgame);
						if (startgame == 1){
							
                            // Sends game settings over CAN and starts game:
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
                // Get current joystick direction, and send it over CAN:
				joystick_direction = getJoyDir();
				IO_send_CAN(joystick_direction);
                
                // Receive score over CAN:
				CAN_message msg;
				CAN_message_recieve(&msg);
				
				if (msg.id == SENDSCORE){
                    // Print score:
					GAME_print_score(msg);
				}
                
				if (msg.id == GAMEOVER){
                    // Print final score if game over:
					GAME_print_finalscore(msg);
					OLED_go_to_pos(6,1);
					OLED_print_sentence("Press any button");
					OLED_go_to_pos(7,10);
					OLED_print_sentence("to go to menu");
                    // Waiting for button to be pressed:
					while (!leftButtonPressed() && !rightButtonPressed() && !joyStickButtonPressed()){}
                    // Go back to main menu, switch state to MENU:
					MENU_goto_main_menu();
					startgame = 0;
					state = MENU;
				}
				break;
		}
	}
}
