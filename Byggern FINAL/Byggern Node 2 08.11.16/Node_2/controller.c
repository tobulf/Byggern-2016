/*
 * controller.c
 *
 * Created: 08.11.2016 17:25:22
 *  Author: tobiasu
 */

/*! \file *********************************************************************
 *
 * \brief Controller.
 *
 *  Initializes regulator, and sets regulator parameters.
 *  Contains regulator.
 *
 *
 *
 *****************************************************************************/


#include "controller.h"
#include "Motor.h"


/**
 * \brief Controller init
 * Set regulator paramters
 * 
 * \param Kp, regulator parameter.
 * \param Ki, regulator paramter.
 * \param pi, regulator paramter.
 * 
 * \return void
 */

void CONTROLLER_init(uint16_t Kp, uint16_t Ki, struct PI *pi){
	 pi->sumError = 0;
	 pi->Kp = Kp;
	 pi->Ki = Ki;

	 pi->maxSumError = 100000;
}

/**
 * \brief PI controller.
 * PI regulator, used to regulate position of shooter, based on slider position.
 *
 * \param setPoint, desired value.
 * \param processValue, current value.
 * \param pi_status, PI-regulator.
 *
 * \return void
 */

int PI_controller(int setPoint, int processValue, PI_data *pi_status){
	uint8_t scaling_factor = 100;
	uint8_t max_i_value = 10000;
	int errors, p_factor, i_factor, temp;
	
    // Calculate error:
	errors = (setPoint - processValue)/2;
    
    // Set P-part of regulator:
	p_factor = pi_status->Kp * errors;
	
	
	// Summing the errors for integral usage:
	temp = pi_status->sumError + errors;
	
    // To avoid overflow:
	if(temp > pi_status->maxSumError){
		i_factor = max_i_value;
		pi_status->sumError = pi_status->maxSumError;
	}
    
    // To avoid overflow:
	if(temp > -pi_status->maxSumError){
		i_factor = -max_i_value;
		pi_status->sumError = -pi_status->maxSumError;
	}
    
    // Set I-part of regulator:
	else{
		pi_status->sumError = temp;
		i_factor = pi_status->Ki * 0.03 * pi_status->sumError;
	}
	
    // Return scaled controller output:
	return ((int)(p_factor+i_factor)/scaling_factor);
}

/**
 * \brief controller reset integrator.
 * Reset integrator error.
 *
 * \param pi_status, regulator.
 *
 * \return void
 */

void CONTROLLER_reset_integrator(PI_data *pi_status){
	pi_status->sumError = 0;
}

