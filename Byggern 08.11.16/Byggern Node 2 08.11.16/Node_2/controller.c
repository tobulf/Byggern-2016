/*
 * controller.c
 *
 * Created: 08.11.2016 17:25:22
 *  Author: tobiasu
 */ 
#include "controller.h"
#include "Motor.h"



void CONTROLLER_init(uint16_t Kp, uint16_t Ki, struct PI *pi){
	 pi->sumError = 0;
	 pi->Kp = Kp;
	 pi->Ki = Ki;
}

double PI_controller(int setPoint, int processValue, PI_data *pi_status){
	int errors, p_factor, i_factor, temp;
	
	errors = setPoint - processValue*0.01;
	p_factor = pi_status->Kp * errors;
	
	
	temp = pi_status->sumError + errors;
	pi_status->sumError = temp;
	i_factor = pi_status->Ki * pi_status->sumError;
	return (p_factor+0.01*i_factor);
}


void CONTROLLER_reset_integrator(PI_data *pi_status){
	pi_status->sumError = 0;
}

