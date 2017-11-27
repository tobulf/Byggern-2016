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


int MOTOR_read_pos(){
	int data=0;
	clear_bit(PORTH, OE);
	clear_bit(PORTH,SEL);
	_delay_us(20);
	data = (PINK << 8); // read msb
	set_bit(PORTH,SEL);
	_delay_us(20);
	data |= PINK;
	encoderReset();
	set_bit(PORTH,OE);
	return data;  
}


void MOTOR_set_speed(Joy_pos joystick_data, struct PI pi){
	//joystick data between -100 and 100
	//printf("%d \n", joystick_data.x);
	joystick_data = convertjoyPos(joystick_data);
	if (joystick_data.x > 0){
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
	int referanceValue = abs(joystick_data.x);
	int measurementValue = MOTOR_read_pos();
	msg[2] = PI_controller(referanceValue, measurementValue, &pi);
	//msg[2] = abs(joystick_data.x); 
	TWI_Start_Transceiver_With_Data(msg, 3);
	
}