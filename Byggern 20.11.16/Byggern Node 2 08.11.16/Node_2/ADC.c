/*
 * ADC.c
 *
 * Created: 25.10.2016 18:52:26
 *  Author: tobiasu
 */ 


#include "ADC.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit ) (reg &= ~(1 << bit))

// Declare interrupt flag for ADC:
uint8_t* ADCFlag_ = NULL;


/*! \brief  ADC init.
 *  Initialize ADC.
 *
 *  \param uint8_t* FlagPtr, set ADCFlag_ to point to FlagPtr.
 *
 *  \return void
 */
void ADC_init(uint8_t* flagPtr){
	ADCFlag_ = flagPtr;
	ADCSRA = 0b11101111; //ADC enable, ADC start conversion, AUTO trigger enable, ADC interrupt enable, prescaler = 128
 	ADMUX = 0b01100000; //internal ARED 2.56V, ADC left adjust, single ended input = ADC0
	//får vi timing problemer, kan det være lurt å bytte fra "Free Running miode" til single conversion mode. Dette medfører at vi må ta inn en MUX-adresse til IR_read. 
}



/**
 * \brief IR read.
 * reading IR value.
 * 
 * 
 * \return uint8_t, IR value.
 */

uint8_t IR_read(){
	return ADCH;
}



/*! \brief  IR blocked.
 *  Checks if IR is blocked.
 *
 *  \param uint8_t* IR_vector, vector containing IR_values.
 *
 *  \return uint8_t, returns 1 if IR is blocked.
 */

uint8_t IR_blocked(uint8_t* IR_vector){
	int sum = 0;
	uint8_t blocked = 0;
	for (int i = 0; i < 4; i++){
		sum += IR_vector[i];
	}
    // Threshold for IR blocked:
	if (sum < 400){
		return 1;
	}
	return 0;

}




/*! \brief  Fill IR vector.
 *  Fills IR vector with IR values.
 *
 *  \param uint8_t* IR_vector, vector containing IR_values.
 *
 *  \return void
 */

void fill_IR_vector(uint8_t* IR_vector){
	uint8_t value = IR_read();
	for (uint8_t i = 0; i < 3; i++){
		IR_vector[i+1] = IR_vector[i];
	}
	IR_vector[0] = value;
}




/*! \brief  reset IR vector.
 *  Empty IR vector.
 *
 *  \param uint8_t* IR_vector, vector containing IR_values to be emptied.
 *
 *  \return void
 */

void reset_IR_vector(uint8_t *IR_vector){
	for (uint8_t i = 0; i < 4; i++){
		IR_vector[i] = 250;
	}
}




/*! \brief  ISR ADC.
 *  Interrupt for ADC. Sets ADC flag.
 *
 *  \param uint8_t* ADC_vect, interrupt vector
 *
 *  \return void
 */

ISR(ADC_vect){
	if(ADCFlag_ != NULL)
		*ADCFlag_ = 1;
 	clear_bit(ADCSRA, ADIF);
}

