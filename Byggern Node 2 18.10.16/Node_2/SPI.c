/*
 * SPI.c
 *
 * Created: 04.10.2016 14:32:54
 *  Author: tobiasu
 */ 



#include "SPI.h"
#include <util/delay.h>
#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit ) (reg &= ~(1 << bit))
#define test_bit(reg, bit ) (reg & (1 << bit))


void SPI_write(uint8_t data)
{
	/* Start transmission */
	SPDR = data;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)))/* _delay_ms(20)*/;

}


uint8_t SPI_read()
{
	/* Start transmission */
	SPDR = 0x00;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}


void SPI_init(){
	
	/* Set MOSI and SCK output, all others input */
	
	set_bit(DDRB, DDB2);
	set_bit(DDRB, DDB1);
	set_bit(DDRB, DDB7); //set chip-select
	set_bit(PORTB, PB3); //internal pull-up
	clear_bit(DDRB,PB3);
	set_bit(DDRB,PB0);
	
		/*
	clear_bit(SPCR, CPOL);
	clear_bit(SPCR, CPHA);
	clear_bit(SPCR, SPR1);
	*/
	
	
	/* Enable SPI, Master, set clock rate fck/16 */
	
	SPCR |= (1<<SPE)|(1<<MSTR); //|(1<<SPR0);
	
	
// 	
// 	//new code
// 	clear_bit(SPCR, SPR0);
// 	clear_bit(SPCR, DORD);
// 	set_bit(SPCR, MSTR);
// 	clear_bit(SPCR, CPOL);
// 	clear_bit(SPCR, CPHA);
// 	set_bit(PORTB, PB0);
}

void SPI_chipSelect(uint8_t select){
	
	if(select == 0){
		clear_bit(PORTB, PB7);
	}
	
	else{
		set_bit(PORTB, PB7);
	}
	
}