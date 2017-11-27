/*
 * controller.c
 *
 * Created: 08.11.2016 17:25:22
 *  Author: tobiasu
 */ 
#include "controller.h"
#include "Motor.h"



/**
 * \brief 
 * 
 * \param Kp
 * \param Ki
 * \param pi
 * 
 * \return void
 */
void CONTROLLER_init(uint16_t Kp, uint16_t Ki, struct PI *pi){
	 pi->sumError = 0;
	 pi->Kp = Kp;
	 pi->Ki = Ki;

	 pi->maxSumError = 100000;
}



int PI_controller(int setPoint, int processValue, PI_data *pi_status){
	uint8_t scaling_factor = 100;
	uint8_t max_i_value = 10000;
	int errors, p_factor, i_factor, temp;
	
	errors = (setPoint - processValue)/2;
	p_factor = pi_status->Kp * errors;
	
	
	
	temp = pi_status->sumError + errors; // multiplying with time constant (20 ms)
	
	if(temp > pi_status->maxSumError){
		i_factor = max_i_value;
		pi_status->sumError = pi_status->maxSumError;
	}
	if(temp > -pi_status->maxSumError){
		i_factor = -max_i_value;
		pi_status->sumError = -pi_status->maxSumError;
	}
	else{
		pi_status->sumError = temp;
		i_factor = pi_status->Ki * 0.03 * pi_status->sumError;
	}
	
	//printf("SumError: %d \n", pi_status->sumError);

	return ((int)(p_factor+i_factor)/scaling_factor);
}


void CONTROLLER_reset_integrator(PI_data *pi_status){
	pi_status->sumError = 0;
}

