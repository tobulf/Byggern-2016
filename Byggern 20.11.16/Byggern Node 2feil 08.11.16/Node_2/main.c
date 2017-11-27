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




	
/**
 * \brief 
 * 
 * \param 
 * 
 * \return int
 */
int main(void)
{	

	
	volatile uint8_t timerFlag = 0;
	volatile uint8_t ADCFlag = 0;
	volatile uint8_t PI_flag = 0;
	volatile int time;
	volatile uint8_t IR_vector[4]; 
	volatile uint8_t score = 0;
	volatile struct PI pi;
	

	
	
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
	//CONTROLLER_init(3, 15, &pi); //kan fjernes, EASY GAME
	CONTROLLER_init(6, 12, &pi); //kan fjernes, NORMAL GAME
	//CONTROLLER_init(12, 11, &pi); // kan fjerns, FUCKED UP GAME
	sei();
	volatile int data = 0;
	
	
	/****** SKAL KJØRES I STARTGAME ***********************************/
	MOTOR_controller_init();

	
	
	while(1)
    {	
		clear_bit(PORTD, PD3);  //hva gjør denne?		
		if (timerFlag){
			CAN_message_recieve(&msg);
			switch (msg.id) {
				case STARTGAME:
					/*set_regulator(msg);
					set_difficulty(msg, &pi);
					set_solenoid_mode(msg);
					set_inverted_mode(msg);
					MOTOR_controller_init();
					//score_reset_time();
					break;
					*/
				case PLAYERINPUT:
					set_servo_x_value(msg);
					MOTOR_controller(msg, &pi);
					trigger_solenoid_mode(msg);
					break;
				
				case JOYSTICK_INIT:
					initialPosition = calibrate(msg);
					break;
				
				default:
					printf("ID invalid");
					break;
			}
			
			timerFlag = 0;
		}
		
		/*
		data = MOTOR_read_pos();
		_delay_ms(10);
		//printf("%d  \n", data);
		*/
		if (ADCFlag){
			fill_IR_vector(&IR_vector);
			//GAME_send_score(score_get_time());
			if (IR_blocked(IR_vector)){
				printf("final score: %d \n", score_get_time());
				//GAME_send_gameover(score_get_time());
			}
		}
		
		
	

		 
    }
	printf("end Main\n");
}




