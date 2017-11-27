/*
 * UARTdriver.c
 *
 * Created: 30.08.2016 15:26:52
 *  Author: tobiasu
 */ 



#include "UARTdriver.h"
#include <avr/io.h>




unsigned char USART_receive(void)
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get and return received data from buffer */
	return UDR0;
}


void USART_transmit( uint8_t data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;

	/* Put data into buffer, sends the data */
	UDR0 = data;
}

void USART_init(void)
{
	unsigned int ubrr = (double)(4915200/(16*9600)-1);
// 		// Kopiert:
	//UBRR0H = (unsigned char) (ubrr>>8);
	//UBRR0L = (unsigned char) ubrr;
// 	

	UBRR0L = 0x1F;	
	UBRR0H = 0;
	// Enabler reciever og transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	
	//Initialiserer aksessering til UCSRC-registeret | Setter til 1stop-bit | Setter til 8-bit data | Setter USART MODE til asynkron. 0 = asynkron, 1 = synkron
	UCSR0C = (1<<URSEL0)|(0<<USBS0)|(3<<UCSZ00)|(0<<UMSEL0);
	
	fdevopen(USART_transmit, USART_receive);
}
