/*
 * Motor.c
 *
 * Created: 08.11.2016 11:22:53
 *  Author: tobiasu
 */ 
#include "joystick.h"
#include "Motor.h"
#include "CAN.h"
#include "CAN_message_decoder.h"
#include "ADC.h"
#define F_CPU 16000000
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "controller.h"
#include <stdio.h>
#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit ) (reg &= ~(1 << bit))
volatile uint8_t CURRENT_REGULATOR = SPEED_REGULATOR;
volatile float SPEED_REG_SCALING = EASYSCALE;
volatile uint8_t CURRENT_INV_MODE = NON_INVERTED_MODE;

void set_regulator(CAN_message msg){
	if (msg.data[0] == POSITION_REGULATOR){
		CURRENT_REGULATOR = POSITION_REGULATOR;
	}
	else if (msg.data[0] == SPEED_REGULATOR){
		CURRENT_REGULATOR = SPEED_REGULATOR;
	}
}

void set_difficulty(CAN_message msg, struct PI *pi){
	if (msg.data[1] == EASYGAME){
		CONTROLLER_init(3, 15, pi);
		SPEED_REG_SCALING = EASYSCALE;
	}
	if (msg.data[1] == NORMALGAME){
		CONTROLLER_init(6, 12, pi);
		SPEED_REG_SCALING = MEDIUMSCALE;
	}
	if (msg.data[1] == HARDGAME){
		CONTROLLER_init(12, 12, pi);
		SPEED_REG_SCALING = HARDSCALE;
	}
}

void set_inverted_mode(CAN_message msg){
	if (msg.data[3] == INVERTED_MODE){
		CURRENT_INV_MODE = INVERTED_MODE;
	}
	else if(msg.data[3] == NON_INVERTED_MODE){
		CURRENT_INV_MODE = NON_INVERTED_MODE;
	}
}

uint8_t get_regulator(){
	return CURRENT_REGULATOR;
}

void DAC_init(){
	
	TWI_Master_Initialise();
	set_bit(DDRD, PD0);
	set_bit(DDRD, PD1); // Setter SCL og SDA pins til output
}

void encoderReset(){
	clear_bit(PORTH,RST); 
	_delay_us(20);
	set_bit(PORTH,RST); 
}

void MOTOR_init(){
	DAC_init(); //initializes DAC
	
	set_bit(DDRH, DIR); //SET DIR output	
	set_bit(DDRH, RST); //SET RST output
	set_bit(DDRH, OE); //SET OE output
	set_bit(DDRH, SEL); //SET SEL output
	
	set_bit(PORTH, EN); //enables encoder
	PORTK = 0x00; //set PORTK input
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


