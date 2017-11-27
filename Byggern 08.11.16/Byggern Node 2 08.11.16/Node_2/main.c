/*
 * Node_2.c
 *
 * Created: 18.10.2016 10:22:46
 *  Author: tobiasu
 */ ;
#include <avr/io.h>
#include "UARTdriver.h"
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#define F_CPU 16000000
#include <util/delay.h>
#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit ) (reg &= ~(1 << bit))
#define test_bit(reg, bit ) (reg & (1 << bit))
#include "MCPdriver.h"
#include "CAN.h"
#include "ADC.h"
#include "controller.h"
#include "solenoid.h"
#include "joystick.h"

#include "timer.h"




	
int main(void)
{	

	
	volatile uint8_t timerFlag = 0;
	volatile uint8_t ADCFlag = 0;
	volatile uint8_t PI_flag = 0;
	volatile int time;
	volatile uint8_t IR_vector[4]; 
	volatile uint8_t score = 0;
	volatile struct PI pi;
	uint8_t ispressed = 0; //solenoid triggermode = semiautomatic
	typedef enum {
		PLAYERINPUT = 0,
		JOYSTICK_INIT
	} ID;
	
	
	//set_bit(MCUCR, SRE);
	//
	//set_bit(MCUCR,SRW10);
	//
	//
	////SRAM_test();
	//set_bit(DDRD, 6); // Write pin Atmega, output
	//set_bit(DDRD, 7); // Read Pin Atmega, output
	//set_bit(DDRE, 1); //Adresse Latch Enable
	//clear_bit(DDRB, 6);
	//clear_bit(DDRB, 7);
	//clear_bit(DDRB, 8);
	USART_init();
	CAN_init();
	ADC_init(&ADCFlag);
	volatile CAN_message msg;
	pwm_timer_init();
	timer_init(&timerFlag);
	score_timer();
	score_reset_time();
	solenoid_init();
	MOTOR_init();
	CONTROLLER_init(1, 0, &pi);
	sei();

	volatile int data = 0;
    while(1)
    {	
		
		
		
		clear_bit(PORTD, PD3);		
		if (timerFlag){
			CAN_message_recieve(&msg);
			switch (msg.id) {
				case PLAYERINPUT:
					set_servo_x_value(msg);
					MOTOR_set_speed(get_joy_pos(msg), pi);
				
				if ((msg.data[5] == 1)  && (ispressed == 0)){
					trigger_solenoid();
					ispressed = 1;
				}
				else if ((msg.data[5] == 0)  && (ispressed == 1)){
					ispressed = 0;
				}
				
				
				
				//	print_joy_pos(message);
				break;
				
				case JOYSTICK_INIT:
				initialPosition = calibrate(&msg);
				break;
				
				default:
				printf("ID invalid");
				break;
			}
			
			timerFlag = 0;
		
		}
	
		data = MOTOR_read_pos();
		_delay_ms(10);
		//printf("%d  \n", data);
			
		if (ADCFlag){
			fill_IR_vector(&IR_vector);
			if (IR_blocked(IR_vector)){
				//printf("final score: %d \n", score_get_time());
				score_reset_time();
			}
		}
		
		
		

		
        //TODO:: Please write your application code 
    }
}




