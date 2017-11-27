/*
 * ADC.h
 *
 * Created: 25.10.2016 18:52:36
 *  Author: tobiasu
 */ 


#ifndef ADC_H_
#define ADC_H_




#include <avr/io.h>

void ADC_init(uint8_t* flagPtr);
uint8_t IR_read();
void fill_IR_vector(uint8_t* IR_vector);
uint8_t IR_blocked(uint8_t* IR_vector);
#endif /* ADC_H_ */
