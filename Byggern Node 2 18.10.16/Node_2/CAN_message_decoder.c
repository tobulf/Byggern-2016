/*
 * CAN_message_decoder.c
 *
 * Created: 18.10.2016 18:11:08
 *  Author: tobiasu
 */ 
#include "CAN_message_decoder.h"

void message_decoder(CAN_message message){
	switch (message.id) {
		case JOYSTICK: 
		print_joy_pos(message);
		break;
		
		case SLIDER_R:
		break;
		
		case SLIDER_L:
		break;
		
		case LEFTBUTTON:
		break;
		
		case RIGHTBUTTON:
		break;
		
		default:
		printf("ID invalid");
		break;
	}
	
	
	
}

