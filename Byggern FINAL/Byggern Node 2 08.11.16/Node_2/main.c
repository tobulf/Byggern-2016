/*
 * Node_2.c
 *
 * Created: 18.10.2016 10:22:46
 *  Author: tobiasu
 */ 


// Defining CPU clock speed:
#define F_CPU 16000000

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
	CONTROLLER_init(6, 12, &pi); 
	sei();
	volatile int data = 0;
	buzzer_init();

	while(1)
    {	
		
		
		
				
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
					//sing(2); underworld melody
					break;
					
				default:
					printf("ID invalid");
					break;
			}
			timerFlag = 0;
		}
	
		data = MOTOR_read_pos();
		_delay_ms(10);
			
		if (ADCFlag){
			fill_IR_vector(&IR_vector);
			GAME_send_score(score_get_score());
			if (IR_blocked(IR_vector)){
				GAME_send_gameover(score_get_score());
			}
		}
    }
}




