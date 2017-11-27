/*
 * joystick.c
 *
 * Created: 20.09.2016 10:11:11
 *  Author: bragesae
 */ 
#include "joystick.h"
#include <avr/io.h>
#include<stdio.h>
#include "ADC.h"
#include "CAN.h"
#define test_bit(reg, bit ) (reg & (1 << bit))
Joy_pos initialPosition;

#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit ) (reg &= ~(1 << bit))




Joy_pos calibrate(){
	Joy_pos newPos;
	newPos.x = ADC_read(1);
	newPos.y = ADC_read(2);
	return newPos;
}


Joy_pos getJoyPos(void){
	Joy_pos position;
	int x = ADC_read(2);
	int y = ADC_read(1);
	
	//
	position.x = x - initialPosition.x;
	position.x *= 100;
	position.x /= initialPosition.x;
	
	position.y = y - initialPosition.y;
	position.y *= 100;
	position.y /= initialPosition.y;

	return position;
}

Slider_pos getSliderPos(void){
	Slider_pos position;
	position.L = ADC_read(3);
	position.R = ADC_read(4);
	return position;
}

int joystick_init(void){
	// set up internal pull-up for PB2
	set_bit(PORTB,PB2);
	clear_bit(DDRB,PB2);
	
	initialPosition = calibrate();
	CAN_message msg;
	msg.id=JOYSTICK_INIT;
	msg.length=2;
	msg.data[0]=initialPosition.x;
	msg.data[1]=initialPosition.y;
	CAN_message_send(&msg);
	return 0;
}

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

void printDir(){
	Joy_pos currentJoyPos = getJoyPos();
	Slider_pos currentSliderPos = getSliderPos();
	int dataX = currentJoyPos.x;
	int dataY = currentJoyPos.y;
	int sliderL = currentSliderPos.L; //returnerer 0
	int sliderR = currentSliderPos.R;
	
	printf("dataX: %d, dataY: %d, sliderR: %d, sliderL: %d \n ", dataX, dataY, ADC_read(4), ADC_read(3));
	
	
}

bool rightButtonPressed(){	
	return (test_bit(PINB, PB0));
}
bool leftButtonPressed(){
	return (test_bit(PINB, PB1));
}

bool joyStickButtonPressed(){
	bool pressed = !(test_bit(PINB, PB2));
	return pressed;
}


void IO_send_CAN(DIR joystick_direction){
	CAN_message msg;
	ID id = PLAYERINPUT;
	uint8_t length = 7;
	msg.id = id;
	msg.length = length;
	msg.data[0] = joystick_direction;
	msg.data[1] = ADC_read(2);
	msg.data[2] = ADC_read(1);
	
	Slider_pos slider = getSliderPos();
	msg.data[3] = slider.R;
	msg.data[4] = slider.L;
	msg.data[5] = rightButtonPressed();
	msg.data[6] = leftButtonPressed();
	CAN_message_send(&msg);
}


