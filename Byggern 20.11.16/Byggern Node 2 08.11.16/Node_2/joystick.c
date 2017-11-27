/*
 * jostick.c
 *
 * Created: 18.10.2016 18:07:03
 *  Author: tobiasu
 */

/*! \file *********************************************************************
 *
 * \brief joystick.
 *
 *  Handles all joystick, slider and button operations on NODE 2.
 *
 *
 *
 *
 *****************************************************************************/

#include "joystick.h"
#include "PWM_TIMER.h"


/**
 * \brief calibrate.
 * returns a calibrated joystick position.
 *
 * \param CAN_message msg, CAN message containing joystick position.
 *
 * \return Joy_pos, joystick position.
 */

Joy_pos calibrate(CAN_message *msg){
	Joy_pos newPos;
	newPos.x = msg->data[0];
	newPos.y = msg->data[1];
	return newPos;
}




/**
 * \brief print joy pos.
 * prints current joystick position.
 *
 * \param CAN_message msg, CAN message containing joystick position.
 *
 * \return void
 */

void print_joy_pos(CAN_message msg){
	Joy_pos position;
	DIR direction = msg.data[DIRECTION];
	position.x = msg.data[JOYSTICK_X];
	position.y = msg.data[JOYSTICK_Y];
	position = convertjoyPos(position);
	printf("direction: %c xpos: %d ypos: %d \n", direction, position.x, position.y);	
}



/**
 * \brief get joy pos.
 * returns current joystick position.
 *
 * \param CAN_message msg, CAN message containing joystick position.
 *
 * \return Joy_pos position, current joystick position
 */

Joy_pos get_joy_pos(CAN_message msg){
	Joy_pos position;
	position.x = msg.data[JOYSTICK_X];
	position.y = msg.data[JOYSTICK_Y];
	return position;
}

/*
 * \brief get slider pos.
 * returns current slider position.
 *
 * \param CAN_message msg, CAN message containing joystick position.
 * \param SLIDER slider, contains current information of sliders.
 *
 * \return void
 */

uint8_t get_slider_pos(CAN_message msg, SLIDER slider){
	if (slider == LEFTSLIDER){
		return msg.data[LEFT_SLIDER];
	}
	else{
		return msg.data[RIGHT_SLIDER];
	}
}


/*
 * \brief convert joy pos
 * convert joystick raw-data to +/- 100 in x/y-space
 *
 * \param msg, CAN message containing joystick position.
 *
 * \return void
 */

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





/*
 * \brief set servo value
 * Set servo to specific position
 *
 * \param msg, CAN message containing slider position.
 *
 * \return void
 */

void set_servo_x_value(CAN_message msg){
	uint8_t slider = get_slider_pos(msg, RIGHTSLIDER);
	int width = (int)(1800 + slider*9);
	pwm_set_width(width);
}




