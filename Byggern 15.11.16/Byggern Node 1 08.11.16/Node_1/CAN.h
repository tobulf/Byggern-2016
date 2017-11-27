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

//ID for CAN:
typedef enum {
	PLAYERINPUT = 0,
	JOYSTICK_INIT,
	STARTGAME,
	GAME_PREFERENCES
} ID;

// Can message struct:
typedef struct{
    // ID of the message, always given by an ENUM.
	unsigned int id;
    // Length of the message, how many bytes the message contains(1-8):
	uint8_t length;
    // Declaration of a 8 byte long array to hold data:
	uint8_t data[8];
}	CAN_message;



void CAN_init();
void CAN_message_send(CAN_message* message);
void CAN_message_recieve(CAN_message *msg);
void Can_CAN_receive();
#endif /* CAN_H_ */
