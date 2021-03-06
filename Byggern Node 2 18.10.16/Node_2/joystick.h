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

typedef struct{
	int x;
	int y;
} Joy_pos;

typedef struct {
	int R;
	int L;
} Slider_pos;

void print_joy_pos(CAN_message msg);
Joy_pos convertjoyPos(Joy_pos pos);


#endif /* JOYSTICK_H_ */