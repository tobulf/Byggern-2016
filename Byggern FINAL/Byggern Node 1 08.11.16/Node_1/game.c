/*
 * game.c
 *
 * Created: 15.11.2016 18:05:43
 *  Author: bragesae
 */ 

#define F_CPU 4915200
#include "game.h"
#include "menu.h"
#include "OLEDdriver.h"
#include "util/delay.h"


// Macros: 
#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit ) (reg &= ~(1 << bit))


/*! \brief  CAN send game settings.
 *  send game settings to CAN.
 *
 *  \param game_settings setting, struct containing game settings.
 *
 *  \return void
 */

void CAN_send_game_settings(game_settings settings){
	CAN_message msg;
	ID id = STARTGAME;
	uint8_t length = 4;
	msg.data[0] = settings.regulator;
	msg.data[1] = settings.difficulty;
	msg.data[2] = settings.solenoid;
	msg.data[3] = settings.inverted;
	msg.id = id;
	msg.length = length;
	CAN_message_send(&msg);
}





/*! \brief  GAME print score.
 *  Print game score to OLED.
 *
 *  \param CAN_message msg, CAN_message containing score.
 *
 *  \return void
 */

void GAME_print_score(CAN_message msg){
	int score = (msg.data[MSB] << 8);
	score |= msg.data[LSB];
	OLED_print_score(abs(score));
	_delay_ms(10);
}


/*! \brief  GAME print finalscore.
 *  print final score to OLED.
 *
 *  \param CAN_message msg, CAN_message containing score.
 *
 *  \return void
 */

void GAME_print_finalscore(CAN_message msg){
	int score = (msg.data[MSB] << 8);
	score |= msg.data[LSB];
	int temp;
	OLED_reset();
	OLED_go_to_pos(0, 22);
	OLED_print_sentence("GAME OVER!");
	OLED_go_to_pos(2, 11);
	OLED_print_sentence("Final Score: ");
	OLED_go_to_pos(3, 44);
	char scorechar[10];
	sprintf(scorechar, "%d", score);
	OLED_print_sentence(scorechar);
	for (int i = 0; i < 5; i++){
		if (score > EEPROM_read_int(i)){
			if (i != 4){
				for (int j = 4; j > i; j--){
					EEPROM_write_int(j, EEPROM_read_int(j-1));
				}
			}
			EEPROM_write_int(i, score);
			OLED_go_to_pos(4, 10);
			OLED_print_sentence("NEW HIGHSCORE!");
			break;
		}
	}
	
}





/*! \brief  GAME print highscores.
 *  print all highscores to OLED
 *
 *  \param void
 *
 *  \return void
 */

void GAME_print_highscores(){
	int score;
	int place;
	OLED_reset();
	OLED_print_sentence("HIGHSCORES:");
	for (int i = 0; i < 5; i++){
		place = i+1;
		score = EEPROM_read_int(i);
		OLED_go_to_pos(i+1, 10);
		OLED_print_int(place);
		OLED_print_sentence(". ");
		OLED_print_int(score);
	}
	
}



/*! \brief  start mario animation.
 *  Start the awesome animation of super mario
 *
 *  \param void
 *
 *  \return void
 */



void GAME_start_mario_animation(){
	OLED_reset();
	OLED_fill_screen();
	OLED_print_floor();
	for (int i = 0; i < 8; i++){
		OLED_go_to_pos(i, 0);
		OLED_clear_char();
		OLED_go_to_pos(i, 3);
		OLED_clear_char();
	}
	OLED_go_to_pos(5, 20);
	OLED_print_mario();
	OLED_print_floor();
	for (int j = 1; j < 3; j++){
	for (int i = 120; i > -1; i--){

		OLED_go_to_pos(5, i);
		OLED_print_pipe();
		OLED_go_to_pos(5, 0);
		OLED_clear_char();
		OLED_go_to_pos(6, 0);
		OLED_clear_char();
		OLED_go_to_pos(5, 3);
		OLED_clear_char();
		OLED_go_to_pos(6, 3);
		OLED_clear_char();
		
		if (i == 35){
			//delete Mario, and print him in other page
			OLED_go_to_pos(5, 20);
			OLED_clear_mario();
			OLED_go_to_pos(4, 20);
			OLED_print_mario();
			
		}
		if (i == 34){
			//delete Mario, and print him in other page
			OLED_go_to_pos(4, 20);
			OLED_clear_mario();
			OLED_go_to_pos(3, 20);
			OLED_print_mario();
		}
		
		if (i == 8){
			//delete Mario, and print him in other page
			OLED_go_to_pos(3, 20);
			OLED_clear_mario();
			OLED_go_to_pos(4, 20);
			OLED_print_mario();
		}
		if (i == 7){
			//delete Mario, and print him in other page
			OLED_go_to_pos(4, 20);
			OLED_clear_mario();
			OLED_go_to_pos(5, 20);
			OLED_print_mario();
		}
		_delay_ms(17);
	}
	
	
	for (int i = 120; i > 19; i--){

		OLED_go_to_pos(5, i);
		OLED_print_monster();
		OLED_go_to_pos(5, 0);
		OLED_clear_char();
		OLED_go_to_pos(6, 0);
		OLED_clear_char();
		OLED_go_to_pos(5, 3);
		OLED_clear_char();
		OLED_go_to_pos(6, 3);
		OLED_clear_char();
		
		if (i == 40){
			//delete Mario, and print him in other page
			OLED_go_to_pos(5, 20);
			OLED_clear_mario();
			OLED_go_to_pos(4, 20);
			OLED_print_mario();
			
		}
		if (i == 39){
			//delete Mario, and print him in other page
			OLED_go_to_pos(4, 20);
			OLED_clear_mario();
			OLED_go_to_pos(3, 20);
			OLED_print_mario();
		}
		
		if (i == 21){
			//delete Mario, and print him in other page
			OLED_go_to_pos(3, 20);
			OLED_clear_mario();
			OLED_go_to_pos(4, 20);
			OLED_print_mario();
		}
		if (i == 20){
			//delete Mario, and print him in other page
			OLED_go_to_pos(4, 20);
			OLED_clear_mario();
			OLED_go_to_pos(5, 20);
			OLED_print_mario();
		}
		_delay_ms(17);
		
		
	}
		
		for (int i = 120; i > 60; i--){

			OLED_go_to_pos(5, i);
			OLED_print_monster();
			OLED_go_to_pos(5, 0);
			OLED_clear_char();
			OLED_go_to_pos(6, 0);
			OLED_clear_char();
			OLED_go_to_pos(5, 3);
			OLED_clear_char();
			OLED_go_to_pos(6, 3);
			OLED_clear_char();
			
			_delay_ms(17);
	}
	char invertedex[8] = {0b00000000,0b00000110,0b01011111,0b01011111,0b00000110,0b00000000,0b00000000,0b00000000};

	_delay_ms(500);
	OLED_go_to_pos(4, 64);
	OLED_write_data(0b11111111);
	OLED_write_data(0b11111001);
	OLED_write_data(0b10100000);
	OLED_write_data(0b10100000);
	OLED_write_data(0b11111001);
	OLED_write_data(0b11111111);
	OLED_write_data(0b11111111);
	OLED_write_data(0b11111111);
	_delay_ms(1000);
	OLED_go_to_pos(4, 64);
	OLED_fill_char();
	_delay_ms(200);
	
			for (int i = 60; i < 128; i++){

				OLED_go_to_pos(5, i);
				OLED_print_monster();
				OLED_go_to_pos(5, 0);
				OLED_clear_char();
				OLED_go_to_pos(6, 0);
				OLED_clear_char();
				OLED_go_to_pos(5, 3);
				OLED_clear_char();
				OLED_go_to_pos(6, 3);
				OLED_clear_char();
				
				_delay_ms(6);
			}
	
	
	
	
	
	
	
	
	
	

}
}



