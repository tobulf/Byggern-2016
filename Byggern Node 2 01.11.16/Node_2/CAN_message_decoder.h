/*
 * Can_message_decoder.h
 *
 * Created: 18.10.2016 18:11:20
 *  Author: tobiasu
 */ 

#include "CAN.h"
typedef enum {
	PLAYERINPUT = 0,
	JOYSTICK_INIT
} ID;


void message_decoder(CAN_message message);
