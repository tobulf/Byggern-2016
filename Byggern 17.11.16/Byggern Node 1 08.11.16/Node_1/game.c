/*
 * game.c
 *
 * Created: 15.11.2016 18:05:43
 *  Author: bragesae
 */ 
#include "game.h"
#include "menu.h"
#include "util/delay.h"
#define F_CPU 4915200


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


void GAME_print_score(CAN_message msg){
	int score = (msg.data[MSB] << 8);
	score |= msg.data[LSB];
	
	//OLED_reset();
	//OLED_print_pig();
	OLED_print_score(abs(score));
	_delay_ms(50);
}


void GAME_print_finalscore(CAN_message msg){
	int score = (msg.data[MSB] << 8);
	score |= msg.data[LSB];
	OLED_reset();
	OLED_print_sentence("GAME OVER!");
	OLED_go_to_pos(2, 11);
	OLED_print_sentence("Final Score: ");
	OLED_go_to_pos(3, 44);
	char scorechar[10];
	sprintf(scorechar, "%d", score);
	OLED_print_sentence(scorechar);	
}




