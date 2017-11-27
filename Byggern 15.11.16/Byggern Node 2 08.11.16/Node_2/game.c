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
void GAME_send_score(uint8_t score){
	CAN_message msg;
	ID id = SENDSCORE;
	uint8_t length = 1;
	msg.id = id;
	msg.length = length;
	msg.data[0] = score;
	CAN_message_send(&msg);
}



void GAME_send_gameover(){
	CAN_message msg;
	ID id = SENDSCORE;
}