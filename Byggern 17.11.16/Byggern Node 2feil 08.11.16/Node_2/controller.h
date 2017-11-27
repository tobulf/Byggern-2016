/*
 * controller.h
 *
 * Created: 08.11.2016 17:25:34
 *  Author: tobiasu
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#define F_CPU 16000000
#include <util/delay.h>


#ifndef CONTROLLER_H_
#define CONTROLLER_H_



typedef struct PI{
	int sumError;
	uint16_t Kp;
	uint16_t Ki;
	int maxSumError;
} PI_data;


void CONTROLLER_init(uint16_t Kp, uint16_t Ki, struct PI *pi);
int PI_controller(int setPoint, int processValue, PI_data *pi_status);

#endif /* CONTROLLER_H_ */