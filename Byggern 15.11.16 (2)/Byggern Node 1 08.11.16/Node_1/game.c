/*
 * game.c
 *
 * Created: 15.11.2016 18:05:43
 *  Author: bragesae
 */ 
#include "game.h"
#include "menu.h"

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









