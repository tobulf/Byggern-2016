/*
 * Motor.c
 *
 * Created: 08.11.2016 11:22:53
 *  Author: tobiasu
 */

/*! \file *********************************************************************
 *
 * \brief Motor DRIVER.
 *
 *  Initialises motor-controlller and DAC, Also function for Writing, reading. 
 *
 *
 *
 *****************************************************************************/

#include "joystick.h"
#include "Motor.h"
#include "CAN.h"
#include "CAN_message_decoder.h"
#include "ADC.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "controller.h"
#include <stdio.h>

// Defining CPU clock:
#define F_CPU 16000000


// Macros:
#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit ) (reg &= ~(1 << bit))

// Static variables for Regulator:
volatile uint8_t CURRENT_REGULATOR = SPEED_REGULATOR;
volatile float SPEED_REG_SCALING = EASYSCALE;
volatile uint8_t CURRENT_INV_MODE = NON_INVERTED_MODE;


/*! \brief  Set regulator.
 *  function to set regulator.
 *
 *  \param CAN_message msg, message containing wich regulator to choose.
 *
 *  \return void
 */

void set_regulator(CAN_message msg){
	if (msg.data[0] == POSITION_REGULATOR){
		CURRENT_REGULATOR = POSITION_REGULATOR;
	}
	else if (msg.data[0] == SPEED_REGULATOR){
		CURRENT_REGULATOR = SPEED_REGULATOR;
	}
}


/*! \brief  Set difficulty.
 *  function to set difficulty.
 *
 *  \param CAN_message msg, message containing difficulty.
 *  \param struct PI *pi, pointer to regulator parameters.
 *
 *  \return void
 */

void set_difficulty(CAN_message msg, struct PI *pi){
	if (msg.data[1] == EASYGAME){
		CONTROLLER_init(3, 8, pi);
		SPEED_REG_SCALING = EASYSCALE;
	}
	if (msg.data[1] == NORMALGAME){
		CONTROLLER_init(6, 8, pi);
		SPEED_REG_SCALING = MEDIUMSCALE;
	}
	if (msg.data[1] == HARDGAME){
		CONTROLLER_init(12, 12, pi);
		SPEED_REG_SCALING = HARDSCALE;
	}
}



/*! \brief  Set inverted mode.
 *  function to set inverted mode.
 *
 *  \param CAN_message msg, message containing bool value for set.
 *
 *  \return void
 */

void set_inverted_mode(CAN_message msg){
	if (msg.data[3] == INVERTED_MODE){
		CURRENT_INV_MODE = INVERTED_MODE;
	}
	else if(msg.data[3] == NON_INVERTED_MODE){
		CURRENT_INV_MODE = NON_INVERTED_MODE;
	}
}




/*! \brief  Get regulator.
 *  function to get current regulator.
 *
 *  \param void
 *
 *  \return void
 */

uint8_t get_regulator(){
	return CURRENT_REGULATOR;
}





/*! \brief  DAC init.
 *  function to initialize DAC.
 *
 *  \param void
 *
 *  \return void
 */
void DAC_init(){
	TWI_Master_Initialise();
    // Sets SCL and SDA pins to output:
	set_bit(DDRD, PD0);
	set_bit(DDRD, PD1);
}



/*! \brief  encoder reset.
 *  function to reset the motor-encoder.
 *
 *  \param void
 *
 *  \return void
 */

void encoderReset(){
	clear_bit(PORTH,RST); 
	_delay_us(20);
	set_bit(PORTH,RST); 
}



/*! \brief  Motor init.
 *  function to initialize Motor-controller
 *
 *  \param void
 *
 *  \return void
 */

void MOTOR_init(){
    //initializes DAC:
	DAC_init();
	//SET DIR output:
	set_bit(DDRH, DIR);
    //SET RST output:
	set_bit(DDRH, RST);
    //SET OE output:
	set_bit(DDRH, OE);
    //SET SEL output:
	set_bit(DDRH, SEL);
	//enables encoder:
	set_bit(PORTH, EN);
    //set PORTK input:
	PORTK = 0x00;
	encoderReset();
	
}

void MOTOR_controller_init(){
	if (CURRENT_REGULATOR == POSITION_REGULATOR){
		
		uint8_t slaveadr = 0b01010000;
		uint8_t cmdbyte = 0x00;
		uint8_t msg[4];
		set_bit(PORTH, DIR);
		msg[0] = slaveadr;
		msg[1] = cmdbyte;
		msg[2] = 100;
		for (int i = 0; i < 10000; i++){
			TWI_Start_Transceiver_With_Data(msg, 3);	
		}
		encoderReset();
		//make slider go all the way to the right, and reset
	}
}

int MOTOR_read_pos(){
	int data=0;
	clear_bit(PORTH, OE);
	clear_bit(PORTH,SEL);
	_delay_us(20);
	data = (PINK << 8); // read msb
	set_bit(PORTH,SEL);
	_delay_us(20);
	data |= PINK;
	if (CURRENT_REGULATOR == SPEED_REGULATOR){
		encoderReset();
	}
	set_bit(PORTH,OE);
	return data;  
}

void MOTOR_controller(CAN_message msg, struct PI *pi){
	if (CURRENT_REGULATOR == SPEED_REGULATOR){
		MOTOR_set_speed_regulator(get_joy_pos(msg), pi);
	}
	else if(CURRENT_REGULATOR == POSITION_REGULATOR){
		MOTOR_set_position_regulator(get_slider_pos(msg, LEFTSLIDER), pi);
	}
}


void MOTOR_set_speed_regulator(Joy_pos joystick_data, struct PI *pi){
	
	
	//joystick data between -100 and 100
	//printf("%d \n", joystick_data.x);
	joystick_data = convertjoyPos(joystick_data);
	if (CURRENT_INV_MODE == NON_INVERTED_MODE){
		if (joystick_data.x > 0){
			set_bit(PORTH, DIR);
		}
		else{
			clear_bit(PORTH, DIR);
		}
	}
	if (CURRENT_INV_MODE == INVERTED_MODE){
		if (joystick_data.x > 0){
			clear_bit(PORTH, DIR);
		}
		else{
			set_bit(PORTH, DIR);;
		}
	}

	uint8_t slaveadr = 0b01010000;
	uint8_t cmdbyte = 0x00;
	uint8_t msg[4];
	msg[0] = slaveadr;
	msg[1] = cmdbyte;
	int referanceValue = abs(joystick_data.x);
	int measurementValue = MOTOR_read_pos();
	msg[2] = SPEED_REG_SCALING * abs(joystick_data.x) / 2; 
	TWI_Start_Transceiver_With_Data(msg, 3);
	
}

void MOTOR_set_position_regulator(uint8_t slider, struct PI *pi){
		//Slider between 0 and 255, need to be between 0 and 8225
		//converting...
		int sliderpos = slider;
		if (CURRENT_INV_MODE == NON_INVERTED_MODE){
			sliderpos = 255 - sliderpos;
		}
		sliderpos *= 32;
		if (sliderpos < MOTOR_read_pos()){
			set_bit(PORTH, DIR);
		}
		else{
			clear_bit(PORTH, DIR);
		}
		uint8_t slaveadr = 0b01010000;
		uint8_t cmdbyte = 0x00;
		uint8_t msg[4];
		msg[0] = slaveadr;
		msg[1] = cmdbyte;
		int referanceValue = sliderpos;
		int measurementValue = (int)(MOTOR_read_pos());
		//printf("ref value %d, meas value %d, \n", referanceValue, measurementValue);
		int absvalue = abs((PI_controller(referanceValue, measurementValue, pi)));
		//printf("value %d, \n", absvalue);
		
		msg[2] = absvalue;
		//msg[2] = slider;
		TWI_Start_Transceiver_With_Data(msg, 3);
}


