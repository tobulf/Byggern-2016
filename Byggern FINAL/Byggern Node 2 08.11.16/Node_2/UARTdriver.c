/*
 * UARTdriver.c
 *
 * Created: 30.08.2016 15:26:52
 *  Author: tobiasu
 */ 



#include "UARTdriver.h"
#include <avr/io.h>




/*! \brief  Recieve USART data.
 *  Polling on data recieval, and returning the data when recieved.
 *
 *  \param void
 *
 *  \return unsigned char, A character of type char
 */

unsigned char USART_receive(void)
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get and return received data from buffer */
	return UDR0;
}



/*! \brief Transmitting data through USART
 *  Polling on transmit buffer, and sending the data into buffer when ready.
 *
 *  \param uint8_t data, desired data to transmit
 *
 *  \return void
 */

void USART_transmit( uint8_t data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;

	/* Put data into buffer, sends the data */
	UDR0 = data;
}





/*! \brief Initializing the USART
 *  Enable reciever and transmitter, also enabling access to USRC REGISTER.
 *
 *  \param void
 *
 *  \return void
 */

void USART_init(void)
{
	unsigned int ubrr = (double)(4915200/(16*9600)-1);
	UBRR0L = 0x067;	
	UBRR0H = 0;
	// Enabler reciever og transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	
	// Setter til 1stop-bit | Setter til 8-bit data | Initialiserer aksessering til UCSRC-registeret
	UCSR0C = (0<<USBS0)|(3<<UCSZ00); //(1<<URSEL0)|
	
	fdevopen(USART_transmit, USART_receive);
}
