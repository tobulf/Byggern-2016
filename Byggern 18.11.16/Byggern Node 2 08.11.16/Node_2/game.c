/*
 * game.c
 *
 * Created: 15.11.2016 16:19:19
 *  Author: bragesae
 */ 

#include "game.h"
#include "CAN.h"


/**
 * \brief send current score via CAN to node 1
 * 
 * \param score, current score
 * 
 * \return void
 */
void GAME_send_score(int score){
	CAN_message msg;
	ID id = SENDSCORE;
	uint8_t length = 2;
	msg.id = id;
	msg.length = length;
	msg.data[LSB] = score;
	uint8_t msb = (score >> 8);
	msg.data[MSB] = (msb);
	CAN_message_send(&msg);
}



void GAME_send_gameover(int score){
	CAN_message msg;
	ID id = GAMEOVER;
	uint8_t length = 2;
	msg.id = id;
	msg.length = length;
	msg.data[LSB] = score;
	uint8_t msb = (score >> 8);
	msg.data[MSB] = (msb);
	CAN_message_send(&msg);
}