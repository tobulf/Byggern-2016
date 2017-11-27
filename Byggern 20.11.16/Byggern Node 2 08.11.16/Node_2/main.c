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
#include <util/delay.h>
#include "MCPdriver.h"
#include "CAN.h"
#include "ADC.h"
#include "controller.h"
#include "solenoid.h"
#include "joystick.h"
#include "buzzer.h"
#include "timer.h"

// Defining CPU clock speed:
#define F_CPU 16000000

// Macros:
#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit ) (reg &= ~(1 << bit))
#define test_bit(reg, bit ) (reg & (1 << bit))



int main(void)
{	

	
	volatile uint8_t timerFlag = 0;
	volatile uint8_t ADCFlag = 0;
	volatile uint8_t PI_flag = 0;
	volatile int time;
	volatile uint8_t IR_vector[4]; 
	volatile uint8_t score = 0;
	volatile struct PI pi;
	

	
	

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

	buzzer_init();
	/*
	while(1){
		set_bit(PORTF, PD2);
		_delay_ms(1);
		clear_bit(PORTF, PD2);
		_delay_ms(1);
	}
	*/
	//sing(3);
	//sing(1);
	//sing(2);
	
	while(1)
    {	
		
		
		
		//clear_bit(PORTD, PD3);		
		if (timerFlag){
			CAN_message_recieve(&msg);
			switch (msg.id) {			
				case STARTGAME:
					set_regulator(msg);
					set_difficulty(msg, &pi);
					set_solenoid_mode(msg);
					set_inverted_mode(msg);
					MOTOR_controller_init();
					reset_IR_vector(&IR_vector);
					score_reset_time();
					break;

				case PLAYERINPUT:
					set_servo_x_value(msg);
					MOTOR_controller(msg, &pi);
					trigger_solenoid_mode(msg);
					break;
		
				case JOYSTICK_INIT:
					initialPosition = calibrate(&msg);
					break;
					
				case CREDITS:
					sing(3);
					break;
					
				case SUPERMARIO:
					sing(1);
					sing(2);
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
			GAME_send_score(score_get_score());
			if (IR_blocked(IR_vector)){
				GAME_send_gameover(score_get_score());
			}
		}
		
		
		

		
        //TODO:: Please write your application code 
    }
}




