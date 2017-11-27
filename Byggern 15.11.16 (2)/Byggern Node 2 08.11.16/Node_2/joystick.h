/*
 * joystick.h
 *
 * Created: 18.10.2016 18:07:12
 *  Author: tobiasu
 */ 

#ifndef JOYSTICK_H_
#define JOYSTICK_H_
#include <stdbool.h>
#include <avr/io.h>
#include "CAN.h"


typedef enum {
	UP,
	DOWN,
	RIGHT,
	LEFT,
	NEUTRAL,
	NONE
} DIR;

typedef enum {
	LEFTSLIDER,
	RIGHTSLIDER
} SLIDER;

typedef enum {
	DIRECTION=0,
	JOYSTICK_X,
	JOYSTICK_Y,
	RIGHT_SLIDER,
	LEFT_SLIDER,
	RIGHT_BUTTON,
	LEFT_BUTTON
}IO_can_index;

typedef struct{
	int x;
	int y;
} Joy_pos;

typedef struct {
	int R;
	int L;
} Slider_pos;
Joy_pos initialPosition;
void print_joy_pos(CAN_message msg);
Joy_pos convertjoyPos(Joy_pos pos);
Joy_pos calibrate(CAN_message *msg);
void set_servo_x_value(CAN_message msg);
uint8_t get_slider_pos(CAN_message msg, SLIDER slider);
Joy_pos get_joy_pos(CAN_message msg);
#endif /* JOYSTICK_H_ */