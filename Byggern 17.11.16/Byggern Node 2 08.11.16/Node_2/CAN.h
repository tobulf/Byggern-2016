/*
 * CAN.h
 *
 * Created: 04.10.2016 14:32:41
 *  Author: tobiasu
 */ 
#include <avr/io.h>
#include <stdio.h>

#ifndef CAN_H_
#define CAN_H_


//kanskje define en enum med ulike ID. Trengs hvis vi skal sende flere enn en msg
#define SCORE_MESSAGE 10

typedef enum {
	PLAYERINPUT = 0,
	JOYSTICK_INIT,
	STARTGAME,
	GAME_PREFERENCES,
	SENDSCORE = 20,
	GAMEOVER
} ID;

typedef struct{
	unsigned int id;
	uint8_t length;
	uint8_t data[8];
}	CAN_message;

void CAN_init();
void CAN_message_send(CAN_message* message);
void CAN_message_recieve(CAN_message *msg);
void IR_send_score(IR_vector);
#endif /* CAN_H_ */