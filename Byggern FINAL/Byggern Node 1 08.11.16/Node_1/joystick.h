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

//Defined enums:


//Direction of joystick:
typedef enum {
	UP,
	DOWN,
	RIGHT,
	LEFT,
	NEUTRAL,
	NONE
} DIR;

//Index of CAN message:
typedef enum {
   DIRECTION=0,
    JOYSTICK_X,
    JOYSTICK_Y,
    RIGHT_SLIDER,
    LEFT_SLIDER,
    RIGHT_BUTTON,
    LEFT_BUTTON
}IO_can_index;


//Defined structs:
typedef struct{
    // Position in x-space:
	int x;
    // Position in y-space:
	int y;	
} Joy_pos;

typedef struct {
    // Position of Right slider:
	int R;
    // Position of Left slider:
	int L;
} Slider_pos;





bool rightButtonPressed();
bool leftButtonPressed();
bool joyStickButtonPressed();
void joystick_init();
Joy_pos calibrate();
Joy_pos getJoyPos();
Slider_pos getSliderPos();
DIR getJoyDir();
void printDir();
void IO_send_CAN(DIR joystick_direction);

#endif /* JOYSTICK_H_ */
