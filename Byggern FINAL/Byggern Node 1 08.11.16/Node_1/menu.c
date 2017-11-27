/*
 * menu.c
 *
 * Created: 27.09.2016 11:02:15
 *  Author: bragesae
 */

/*! \file *********************************************************************
 *
 * \brief MENU system.
 *
 *  Complete menu system, with all choices and functionality of the game. 
 *
 *
 *
 *****************************************************************************/

#include "menu.h"
#include "util/delay.h"
#include "joystick.h"
#include "OLEDdriver.h"
#include "game.h"
#include "CAN.h"

// Static menu options:
static  struct menuScreen main, newGame, HighScores, Options, currentMenu, easyGame, normalGame, hardGame, Credits, Controller, SpeedController,
PositionController, Mario, SolenoidMode, InvertedMode, SolenoidAuto, SolenoidSemi, SolenoidToggleAuto, NonInverted, Inverted, resetHighscore;


// Bookmark for current page:
uint8_t markerPage;



/*! \brief  Initialising menu.
 *  initializes menu, with all options.
 *
 *  \param void
 *
 *  \return void
 */

void MENU_init_menu(){
	
	main.name = "MainMenu";
	main.parent = NULL;
	main.child[1] = &newGame;
	main.child[2] = &HighScores;
	main.child[3] = &Options;
	main.child[4] = &Credits;
	main.nmbrChilds = 4;
	main.menu_ID = mainID;
	
	//main menu children
	newGame.name = "New Game";
	newGame.parent = &main;
	newGame.child[1] = &easyGame;
	newGame.child[2] = &normalGame;
	newGame.child[3] = &hardGame;
	newGame.nmbrChilds = 3;	
	newGame.menu_ID = newGameID;
	
		//New game children
 		easyGame.name = "Easy";
 		easyGame.parent = &newGame;
 		easyGame.nmbrChilds = 0;
		easyGame.menu_ID = easyGameID;
 		
 		normalGame.name = "Normal";
 		normalGame.parent = &newGame;
 		normalGame.nmbrChilds = 0;
 		normalGame.menu_ID = normalGameID;
		 
 		hardGame.name = "Hard";
 		hardGame.parent = &newGame;
 		hardGame.nmbrChilds = 0;
		hardGame.menu_ID = hardGameID;
	
	HighScores.name = "High Scores";
	HighScores.parent = &main;
	HighScores.nmbrChilds = 0;
	HighScores.menu_ID = HighScoresID;

	Options.name = "Options";
	Options.parent = &main;
	Options.child[1] = &Controller;
	Options.child[2] = &SolenoidMode;
	Options.child[3] = &InvertedMode;
	Options.child[4] = &resetHighscore;
	Options.child[5] = &Mario;
	Options.nmbrChilds = 5;
	Options.menu_ID = OptionsID;

		//options children
		Controller.name = "Controller sel";
		Controller.parent = &Options;
		Controller.child[1] = &SpeedController;
		Controller.child[2] = &PositionController;
		Controller.nmbrChilds = 2;
		Controller.menu_ID = ControllerID;
			
			//Controller children
			SpeedController.name = "Speed";
			SpeedController.parent = &Controller;
			SpeedController.nmbrChilds = 0;
			SpeedController.menu_ID = SpeedControllerID;

			PositionController.name = "Position";
			PositionController.parent = &Controller;
			PositionController.nmbrChilds = 0;
			PositionController.menu_ID = PositionControllerID;
			
		
		SolenoidMode.name = "Solenoid Mode";
		SolenoidMode.parent = &Options;
		SolenoidMode.child[1] = &SolenoidSemi;
		SolenoidMode.child[2] = &SolenoidAuto;
		SolenoidMode.child[3] = &SolenoidToggleAuto;
		SolenoidMode.nmbrChilds = 3;
		SolenoidMode.menu_ID = SolenoidModeID;
	
			//SolenoidMode children
			SolenoidSemi.name = "Semi Auto";
			SolenoidSemi.parent = &SolenoidMode;
			SolenoidSemi.nmbrChilds = 0;
			SolenoidSemi.menu_ID = SolenoidSemiID;
			
			SolenoidAuto.name = "Auto";
			SolenoidAuto.parent = &SolenoidMode;
			SolenoidAuto.nmbrChilds = 0;
			SolenoidAuto.menu_ID = SolenoidAutoID;
	
			SolenoidToggleAuto.name = "Toggle Auto";
			SolenoidToggleAuto.parent = &SolenoidMode;
			SolenoidToggleAuto.nmbrChilds = 0;
			SolenoidToggleAuto.menu_ID = SolenoidToggleAutoID;
	
		InvertedMode.name = "Inverted Mode";
		InvertedMode.parent = &Options;
		InvertedMode.child[1] = &NonInverted;
		InvertedMode.child[2] = &Inverted;
		InvertedMode.nmbrChilds = 2;
		InvertedMode.menu_ID = InvertedModeID;
		
			//InvertedMode children
			NonInverted.name = "Non Inverted";
			NonInverted.parent = &InvertedMode;
			NonInverted.nmbrChilds = 0;
			NonInverted.menu_ID = NonInvertedID;
			
			Inverted.name = "Inverted";
			Inverted.parent = &InvertedMode;
			Inverted.nmbrChilds = 0;
			Inverted.menu_ID = InvertedID;
			
		resetHighscore.name = "Reset Score";
		resetHighscore.parent = &Options;
		resetHighscore.nmbrChilds = 0;
		resetHighscore.menu_ID = resetHighscoreID;
	
	Mario.name = "Mario";
	Mario.parent = &Options;
	Mario.nmbrChilds = 0;
	Mario.menu_ID = MarioID;
	
	Credits.name = "Credits";
	Credits.parent = &main;
	Credits.nmbrChilds = 0;
	Credits.menu_ID = CreditsID;	
	
	currentMenu = main;
	OLED_go_to_pos(1, 0);
	markerPage = 1;	
	OLED_print_char((uint8_t)(36));	
}


/*! \brief  Print menu.
 *  Prints menu to OLED-display.
 *
 *  \param void
 *
 *  \return void
 */

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



/*! \brief  Menu selectline.
 *  Highlights given option.
 *
 *  \param uint8_t line,goes to designated line higligths this line.
 *
 *  \return void
 */

void MENU_selectLine(uint8_t line){
	OLED_go_to_pos(line, 0);
	OLED_print_char((uint8_t)(36));
}



/*! \brief  clear line.
 *  clears designated line.
 *
 *  \param uint8_t line, goes to designated line, and removes highlight.
 *
 *  \return void
 */

void MENU_clearLine(uint8_t line){
	OLED_go_to_pos(line, 0);
	OLED_clear_char();
}



/*! \brief  navigate.
 *  Function for navigating the menu.
 *
 *  \param DIR direction, direction for navigation.
 *
 *  \return void
 */

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
				MENU_goto_parent_menu();		
		}
	}
}




/*! \brief  MENU goto parent menu.
 *  Function for navigating to the parent menu.
 *
 *  \param void
 *
 *  \return void
 */

void MENU_goto_parent_menu(){
	currentMenu = *currentMenu.parent;
	OLED_reset();
	MENU_print_menu();
	markerPage = 1;
	MENU_selectLine(markerPage);
}



/*! \brief  MENU goto main menu.
 *  Function for navigating to the main menu.
 *
 *  \param void
 *
 *  \return void
 */

void MENU_goto_main_menu(){
	currentMenu = main;
	OLED_reset();
	MENU_print_menu();
	markerPage = 1;
	MENU_selectLine(markerPage);
}



/*! \brief  Select action.
 *  selects an option in the menu.
 *
 *  \param void
 *
 *  \return void
 */

void MENU_selectAction(game_settings *settings, uint8_t *startgame){
    if (currentMenu.nmbrChilds == 0){
		CAN_message msg;
		ID id = 1;
		int length = 1;
        switch (currentMenu.menu_ID){
		
            case HighScoresID:
                GAME_print_highscores();
                break;
                
            case easyGameID:
                settings->difficulty = 0;
                *startgame = 1;
                break;
                
            case normalGameID:
                settings->difficulty = 1;
                *startgame = 1;
                break;
            
            case hardGameID:
                settings->difficulty = 2;
                *startgame = 1;
                break;
            
            case resetHighscoreID:
                EEPROM_init();
                MENU_goto_main_menu();
                break;
            
            case MarioID:
                id = SUPERMARIO;
                length = 1;
                msg.id = id;
                msg.length = length;
                msg.data[0] = 0;
                CAN_message_send(&msg);
				GAME_start_mario_animation();
				MENU_goto_main_menu();
                break;
            
            case CreditsID:
                OLED_reset();
                id = CREDITS;
                length = 1;
                msg.id = id;
                msg.length = length;
                msg.data[0] = 0;
                CAN_message_send(&msg);
                OLED_credits();
                break;
            
            case SpeedControllerID:
                settings->regulator = 1;
                MENU_goto_parent_menu();
                MENU_goto_parent_menu();
                break;
            
            case PositionControllerID:
                settings->regulator = 0;
                MENU_goto_parent_menu();
                MENU_goto_parent_menu();
                break;
                
            case SolenoidSemiID:
                settings->solenoid = 0;
                MENU_goto_parent_menu();
                MENU_goto_parent_menu();
                break;
                
            case SolenoidAutoID:
                settings->solenoid = 1;
                MENU_goto_parent_menu();
                MENU_goto_parent_menu();
                break;
                
            case SolenoidToggleAutoID:
                settings->solenoid = 2;
                MENU_goto_parent_menu();
                MENU_goto_parent_menu();
                break;
                
            case NonInvertedID:
                settings->inverted = 0;
                MENU_goto_parent_menu();
                MENU_goto_parent_menu();
                break;
                
            case InvertedID:
                settings->inverted = 1;
                MENU_goto_parent_menu();
                MENU_goto_parent_menu();
                break;
                
            default:
                break;
        }
    }
}

