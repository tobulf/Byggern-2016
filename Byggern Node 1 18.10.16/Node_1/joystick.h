/*
 * joystick.h
 *
 * Created: 20.09.2016 10:11:24
 *  Author: bragesae
 */ 



#ifndef JOYSTICK_H_
#define JOYSTICK_H_
#include <stdbool.h>
#include <avr/io.h>

typedef enum {
	JOYSTICK = 0,
	SLIDER_R,
	SLIDER_L,
	LEFTBUTTON,
	RIGHTBUTTON,
	NONE
} ID;


typedef enum {
	UP,
	DOWN,
	RIGHT,
	LEFT,
	NEUTRAL
} DIR;

typedef struct{
	int x;
	int y;	
} Joy_pos;

typedef struct {
	int R;
	int L;
} Slider_pos;

bool rightButtonPressed();
bool leftButtonPressed();
bool joyStickButtonPressed();

int joystick_init(void);
Joy_pos calibrate();
Joy_pos getJoyPos();
Slider_pos getSliderPos();
DIR getJoyDir();
void printDir();

void joystick_send_CAN(DIR joystick_direction);

#endif /* JOYSTICK_H_ */