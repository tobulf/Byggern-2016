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

uint8_t* ADCFlag_ = NULL;

/*
void ADC_init(uint8_t* flagPtr){
	ADCFlag_ = flagPtr;
	ADMUX = 0b11100000;
	ADCSRA = 0b11101111;
	
}
*/

void ADC_init(uint8_t* flagPtr){
	ADCFlag_ = flagPtr;
	ADCSRA = 0b11101111; //ADC enable, ADC start conversion, AUTO trigger enable, ADC interrupt enable, prescaler = 128
 	ADMUX = 0b01100000; //internal ARED 2.56V, ADC left adjust, single ended input = ADC0
	//får vi timing problemer, kan det være lurt å bytte fra "Free Running miode" til single conversion mode. Dette medfører at vi må ta inn en MUX-adresse til IR_read. 
}



/**
 * \brief 
 * 
 * 
 * \return uint8_t
 */
uint8_t IR_read(){
	return ADCH;
}




uint8_t IR_blocked(uint8_t* IR_vector){
	int sum = 0;
	uint8_t blocked = 0;
	for (int i = 0; i < 4; i++){
		sum += IR_vector[i];
	}
	if (sum < 400){
		return 1;
	}
	return 0;

}

void fill_IR_vector(uint8_t* IR_vector){
	uint8_t value = IR_read();
	for (uint8_t i = 0; i < 3; i++){
		IR_vector[i+1] = IR_vector[i];
	}
	IR_vector[0] = value;
}


ISR(ADC_vect){
	if(ADCFlag_ != NULL)
		*ADCFlag_ = 1;
 	clear_bit(ADCSRA, ADIF);
}

