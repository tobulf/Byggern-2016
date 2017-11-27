/*
 * menu.c
 *
 * Created: 27.09.2016 11:02:15
 *  Author: bragesae
 */ 
#include "menu.h"
#include "util/delay.h"
#include "joystick.h"
#include "OLEDdriver.h"
static  struct menuScreen main, newGame, HighScores, Options, currentMenu, easyGame, normalGame, hardGame, Credits, Brightness, Sound;

uint8_t markerPage;

void MENU_init_menu(){
	
	main.name = "MainMenu";
	main.parent = NULL;
	main.child[1] = &newGame;
	main.child[2] = &HighScores;
	main.child[3] = &Options;
	main.child[4] = &Credits;
	main.menuIndex = 0;
	main.nmbrChilds = 4;
	
	newGame.name = "New Game";
	newGame.parent = &main;
	newGame.child[1] = &easyGame;
	newGame.child[2] = &normalGame;
	newGame.child[3] = &hardGame;
	newGame.nmbrChilds = 3;
	newGame.menuIndex = 1;
	
	
 	easyGame.name = "Easy";
 	easyGame.parent = &newGame;
 	easyGame.nmbrChilds = 0;
 	easyGame.menuIndex = 11;
 	
 	normalGame.name = "Normal";
 	normalGame.parent = &newGame;
 	normalGame.nmbrChilds = 0;
 	normalGame.menuIndex = 12;
 	
 	hardGame.name = "Hard";
 	hardGame.parent = &newGame;
 	hardGame.nmbrChilds = 0;
 	hardGame.menuIndex = 13;
	
	
	HighScores.name = "High Scores";
	HighScores.parent = &main;
	HighScores.nmbrChilds = 0;
	HighScores.menuIndex = 2;
	
	Options.name = "Options";
	Options.parent = &main;
	Options.child[1] = &Brightness;
	Options.child[2] = &Sound;
	Options.nmbrChilds = 2;
	Options.menuIndex = 3;
	
	Brightness.name = "Brightness";
	Brightness.parent = &Options;
	Brightness.nmbrChilds = 0;
	Brightness.menuIndex = 31;
	
	Sound.name = "Sound";
	Sound.parent = &Options;
	Sound.nmbrChilds = 0;
	Sound.menuIndex = 32;
	
	Credits.name = "Credits";
	Credits.parent = &main;
	Credits.nmbrChilds = 0;
	Credits.menuIndex = 4;
		
	
	currentMenu = main;
	OLED_go_to_pos(1, 0);
	markerPage = 1;	
	OLED_print_char((uint8_t)(36));

	
	
}



void MENU_print_menu(){
	OLED_go_to_pos(0, 10);
	OLED_print_sentence("*");
	OLED_print_sentence(currentMenu.name);
	OLED_print_sentence("*");
	for (int i = 1; i < currentMenu.nmbrChilds+1; i++){
		
		OLED_go_to_pos(i, 10);
		OLED_print_sentence(currentMenu.child[i]->name);

	}

}

void MENU_selectLine(uint8_t line){
	OLED_go_to_pos(line, 0);
	OLED_print_char((uint8_t)(36));
}

void MENU_clearLine(uint8_t line){
	OLED_go_to_pos(line, 0);
	OLED_clear_char();
}

void MENU_navigate(DIR direction){
	
	if (direction == UP){
		if (markerPage > 1){
			MENU_clearLine(markerPage);
			markerPage--;
			MENU_selectLine(markerPage);	
		}
	}
	
	else if (direction == DOWN){
		if (markerPage < currentMenu.nmbrChilds){
			MENU_clearLine(markerPage);
			markerPage++;
			MENU_selectLine(markerPage);
	}	
}
		else if(direction == RIGHT){
			if ((markerPage <= currentMenu.nmbrChilds) && (markerPage != 0)){
				currentMenu = *currentMenu.child[markerPage];
				OLED_reset();
				MENU_print_menu();
				markerPage = 1;
				MENU_selectLine(markerPage);
				
			}
		}
		else if(direction == LEFT){
			if (currentMenu.parent != NULL){
				currentMenu = *currentMenu.parent;
				OLED_reset();
				MENU_print_menu();
				markerPage = 1;
				MENU_selectLine(markerPage);
				
			
		}
	}
}
	
void MENU_selectAction(){
	if (currentMenu.nmbrChilds == 0){
		if (currentMenu.name == "High Scores"){
			OLED_print_sentence("high scores");
			//print highscores
		}
		if (currentMenu.name == "Easy"){
			OLED_print_sentence("high scores");
			//start game
		}
		if (currentMenu.name == "Normal"){
			OLED_print_sentence("Normal");
			//start game
		}
		if (currentMenu.name == "Hard"){
			OLED_print_sentence("Hard");
			//start game
		}
		if (currentMenu.name == "Credits"){
			OLED_reset();
			OLED_credits();
		}
		

		
		
	}

}
