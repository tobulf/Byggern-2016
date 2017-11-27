/*
 * joystick.c
 *
 * Created: 20.09.2016 10:11:11
 *  Author: bragesae
 */

/*! \file *********************************************************************
 *
 * \brief IO board driver.
 *
 *  Initialises joystick, Also function for calibration, extracting position of sliders and joystick. 
 *  Also function for sending all user input over CAN.
 *
 *
 *
 *****************************************************************************/


#include "joystick.h"
#include <avr/io.h>
#include<stdio.h>
#include "ADC.h"
#include "CAN.h"

//defining macros:
#define test_bit(reg, bit ) (reg & (1 << bit))
#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit ) (reg &= ~(1 << bit))

// For saving initial position:
Joy_pos initialPosition;



/*! \brief  Calibrating Joystick.
 *  Reads initial position of joystick and saves it.
 *
 *  \param void
 *
 *  \return Joy_pos, Returns a struct: Joy_pos, containing calibration values.
 */

Joy_pos calibrate(){
	Joy_pos newPos;
	newPos.x = ADC_read(1);
	newPos.y = ADC_read(2);
	return newPos;
}




/*! \brief  Retrieving Joystick position
 *  Reads the current position and returns it.
 *
 *  \param void
 *
 *  \return Joy_pos, Returns a struct: Joy_pos, containing position.
 */

Joy_pos getJoyPos(){
	Joy_pos position;
	int x = ADC_read(2);
	int y = ADC_read(1);
	
	//converts positions to +- 100
	position.x = x - initialPosition.x;
	position.x *= 100;
	position.x /= initialPosition.x;
	
	position.y = y - initialPosition.y;
	position.y *= 100;
	position.y /= initialPosition.y;

	return position;
}




/*! \brief  Retrieving slider position.
 *  Reads sliders current position and returns it.
 *
 *  \param void
 *
 *  \return Slider_pos, Returns a struct: Slider_pos, containing current values.
 */

Slider_pos getSliderPos(){
	Slider_pos position;
	position.L = ADC_read(3);
	position.R = ADC_read(4);
	return position;
}




/*! \brief  Initializing IO board
 *  Initializes IO board, calibrates joystick and sends calibration values to NODE2.
 *
 *  \param void
 *
 *  \return void
 */

void joystick_init(){
	// Set up internal pull-up for PB2 Joystickbutton:
	set_bit(PORTB,PB2);
	clear_bit(DDRB,PB2);
	//Saves initial position for calibration:
	initialPosition = calibrate();
    //Send initilizing CAN message for NODE2:
	CAN_message msg;
	msg.id=JOYSTICK_INIT;
	msg.length=2;
	msg.data[0]=initialPosition.x;
	msg.data[1]=initialPosition.y;
	CAN_message_send(&msg);
}




/*! \brief  Retrieving Joystick-direction
 *  Converts joystick position to Directions, UP, DOWN, RIGHT, LEFT, Neutral
 *
 *  \param void
 *
 *  \return DIR, returns enum DIR.
 */

DIR getJoyDir(){
	Joy_pos position = getJoyPos();
	if (position.x > 50){
		return RIGHT;
	}
	else if (position.x < -50){
		return LEFT;
	}
	else if (position.y < -50){
		return DOWN;
	}
	else if (position.y > 50){
		return UP;
	}
	else if ((abs(position.x) < 5) && (abs(position.y) < 5)){
		return NEUTRAL;
	}
	else{
		return NONE;
	}

}




/*! \brief  prints all IO input
 *
 *  \param void
 *
 *  \return void
 */

void printDir(){
	Joy_pos currentJoyPos = getJoyPos();
	Slider_pos currentSliderPos = getSliderPos();
	int dataX = currentJoyPos.x;
	int dataY = currentJoyPos.y;
	int sliderL = currentSliderPos.L; //returnerer 0
	int sliderR = currentSliderPos.R;
	
	printf("dataX: %d, dataY: %d, sliderR: %d, sliderL: %d \n ", dataX, dataY, ADC_read(4), ADC_read(3));
	
	
}



/*! \brief  Rightbutton
 *  Checks if Rightbutton is pressed
 *
 *  \param void
 *
 *  \return bool, returns true or false.
 */

bool rightButtonPressed(){	
	return (test_bit(PINB, PB0));
}



/*! \brief  leftbutton
 *  Checks if leftbutton is pressed
 *
 *  \param void
 *
 *  \return bool, returns true or false.
 */

bool leftButtonPressed(){
	return (test_bit(PINB, PB1));
}



/*! \brief joystickbutton
 *  Checks if joystickbutton is pressed
 *
 *  \param void
 *
 *  \return bool, returns true or false.
 */

bool joyStickButtonPressed(){
	bool pressed = !(test_bit(PINB, PB2));
	return pressed;
}




/*! \brief  Send IO input to NODE2
 *  Reads, stores and sends all IO-input to NODE2.
 *
 *  \param DIR joystick_direction, takes in to send to NODE2.
 *
 *  \return void
 */

void IO_send_CAN(DIR joystick_direction){
	CAN_message msg;
	ID id = PLAYERINPUT;
	uint8_t length = 7;
	msg.id = id;
	msg.length = length;
	msg.data[DIRECTION] = joystick_direction;
	msg.data[JOYSTICK_X] = ADC_read(2);
	msg.data[JOYSTICK_Y] = ADC_read(1);
	//Saves and sends sliderposition:
	Slider_pos slider = getSliderPos();
	msg.data[RIGHT_SLIDER] = slider.R;
	msg.data[LEFT_SLIDER] = slider.L;
	msg.data[RIGHT_BUTTON] = rightButtonPressed();
	msg.data[LEFT_BUTTON] = leftButtonPressed();
	CAN_message_send(&msg);
}


