/*
 * ADC.h
 *
 * Created: 13.09.2016 14:23:39
 *  Author: tobiasu
 */ 


#ifndef ADC_H_
#define ADC_H_


	
void ADC_init();
uint8_t ADC_read(uint8_t channel);

uint8_t adcflag;
ISR(INT0_vect);

#endif /* ADC_H_ */