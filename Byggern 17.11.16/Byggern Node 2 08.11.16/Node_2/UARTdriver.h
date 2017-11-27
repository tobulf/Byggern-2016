/*
 * UARTdriver.h
 *
 * Created: 30.08.2016 15:26:30
 *  Author: tobiasu
 */ 

#include <avr/io.h>

#define BAUD 9600;
#define MYUBRR FOSC/16/BAUD-1;

#ifndef UARTDRIVER_H_
#define UARTDRIVER_H_

void USART_init(void);
unsigned char USART_receive(void);
void USART_transmit( uint8_t data );


#endif /* UARTDRIVER_H_ */