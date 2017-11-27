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


struct can_message{
	unsigned int id;
	uint8_t length;
	uint8_t data[8];
};




#endif /* CAN_H_ */