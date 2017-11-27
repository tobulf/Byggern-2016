/*
 * SPI.c
 *
 * Created: 04.10.2016 14:32:54
 *  Author: tobiasu
 */ 



#include "SPI.h"

#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit ) (reg &= ~(1 << bit))
#define test_bit(reg, bit ) (reg & (1 << bit))


uint8_t SPI_transmit(uint8_t data)
{
	/* Start transmission */
	SPDR = data;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}



void SPI_init(){
	
	/* Set MOSI and SCK output, all others input */
	
	set_bit(DDRB, DDB5);
	set_bit(DDRB, DDB7);
	set_bit(DDRB, DDB4); //set chip-select
	
	
	
	/* Enable SPI, Master, set clock rate fck/16 */
	
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}