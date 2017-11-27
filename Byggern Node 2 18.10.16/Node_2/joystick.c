/*
 * jostick.c
 *
 * Created: 18.10.2016 18:07:03
 *  Author: tobiasu
 */ 
#include "joystick.h"

void print_joy_pos(CAN_message msg){
	Joy_pos position;
	DIR direction = msg.data[0];
	position.x = msg.data[1];
	position.y = msg.data[2];
	position = convertjoyPos(position);
	printf("direction: %c xpos: %d ypos: %d \n", direction, position.x, position.y);	
}

Joy_pos convertjoyPos(Joy_pos pos){
	Joy_pos position;
	position.x = pos.x - 129;
	position.x *= 100;
	position.x /= 129;
	   
	position.y = pos.y - 128;
	position.y *= 100;
	position.y /= 128;
	return position;
}