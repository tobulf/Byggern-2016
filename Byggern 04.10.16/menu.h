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


void MENU_init_menu();
void MENU_navigate(DIR direction);
void MENU_selectLine(uint8_t line);
void MENU_clearLine(uint8_t line);
void MENU_selectAction();
struct menuScreen{
	
	char * name;
	struct menuScreen *parent;
	struct menuScreen  *child[6];
	uint8_t nmbrChilds;
	uint8_t menuIndex;
	
	
	};

void MENU_goto_menu(void);
void MENU_print_menu();
#endif /* MENU_H_ */