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

typedef struct{
	unsigned int id;
	uint8_t length;
	uint8_t data[8];
}	CAN_message;

void CAN_init();
void CAN_message_send(CAN_message* message);
void CAN_message_recieve(CAN_message *msg);

#endif /* CAN_H_ */