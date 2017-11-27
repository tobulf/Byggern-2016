/*
 * menu.h
 *
 * Created: 27.09.2016 11:02:44
 *  Author: bragesae
 */ 
#include "joystick.h"
#include <stdbool.h>
#ifndef MENU_H_
#define MENU_H_
#include <stdbool.h>
#include <avr/io.h>
#include<stdio.h>
#include "OLEDdriver.h"

#define POSITION_REGULATOR 0
#define SPEED_REGULATOR 1
#define NON_INVERTED_MODE 0
#define INVERTED_MODE 1
#define SOLENOID_AUTO 0
#define SOLENOID_SEMI 1
#define SOLENOID_SEMI_AUTO 2


// Structs:
struct menuScreen{
	// Name of option:
	char * name;
	// Enum ID
	
	// Pointer to Parent option:
	struct menuScreen *parent;
	// Pointer to sub-options:
	struct menuScreen  *child[6];
	// Number of suboptions:
	uint8_t nmbrChilds;
	};
	
typedef struct startgame_settings{
	uint8_t regulator;
	uint8_t difficulty;
	uint8_t solenoid;
	uint8_t inverted;
} game_settings;

void MENU_goto_menu(void);
void MENU_print_menu();
void MENU_init_menu();
void MENU_navigate(DIR direction);
void MENU_selectLine(uint8_t line);
void MENU_clearLine(uint8_t line);
void MENU_selectAction(game_settings *settings, uint8_t *startgame);
void MENU_goto_parent_menu();
void MENU_goto_main_menu();
#endif /* MENU_H_ */