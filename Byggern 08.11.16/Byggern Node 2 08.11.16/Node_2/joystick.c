/*
 * jostick.c
 *
 * Created: 18.10.2016 18:07:03
 *  Author: tobiasu
 */ 
#include "joystick.h"
#include "PWM_TIMER.h"
Joy_pos calibrate(CAN_message *msg){
	Joy_pos newPos;
	newPos.x = msg->data[0];
	newPos.y = msg->data[1];
	return newPos;
}


void print_joy_pos(CAN_message msg){
	Joy_pos position;
	DIR direction = msg.data[0];
	position.x = msg.data[1];
	position.y = msg.data[2];
	position = convertjoyPos(position);
	printf("direction: %c xpos: %d ypos: %d \n", direction, position.x, position.y);	
}

Joy_pos get_joy_pos(CAN_message msg){
	Joy_pos position;
	position.x = msg.data[1];
	position.y = msg.data[2];
	return position;
}

Joy_pos convertjoyPos(Joy_pos pos){
	Joy_pos position;
	position.x = pos.x - initialPosition.x;
	position.x *= 100;
	position.x /= initialPosition.x;
	   
	position.y = pos.y - initialPosition.y;
	position.y *= 100;
	position.y /= initialPosition.y;
	return position;
}

void set_servo_x_value(CAN_message msg){
	uint8_t slider = msg.data[3];
	int width = (int)(1800 + slider*9);
	pwm_set_width(width);
}