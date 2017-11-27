/*
 * CAN_message_decoder.c
 *
 * Created: 18.10.2016 18:11:08
 *  Author: tobiasu
 */ 
#include "CAN_message_decoder.h"
#include "joystick.h"
#include "solenoid.h"

void message_decoder(CAN_message message){
	switch (message.id) {
		case PLAYERINPUT: 
		set_servo_x_value(message);
		
		if (message.data[5] == 1){
			trigger_solenoid();
		}
	//	print_joy_pos(message);
		break;
				case JOYSTICK_INIT:
		initialPosition = calibrate(&message);
		break;
		
		default:
		printf("ID invalid");
		break;
	}
	
}

