/*
 * SPI.c
 *
 * Created: 04.10.2016 14:32:54
 *  Author: tobiasu
 */

/*! \file *********************************************************************
 *
 * \brief SPI Driver.
 *
 *  Initialises SPI, Also function for Writing, reading and SPI-chipselect.
 *
 *
 *
 *****************************************************************************/



#include "SPI.h"

// Macros:
#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit ) (reg &= ~(1 << bit))
#define test_bit(reg, bit ) (reg & (1 << bit))



/*! \brief  SPI write.
 *  function for writing to SPI register.
 *
 *  \param uint8_t data, data to be written.
 *
 *  \return void
 */

void SPI_write(uint8_t data)
{
	/* Start transmission */
	SPDR = data;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));

}




/*! \brief  SPI read.
 *  function for reading from SPI register.
 *
 *  \param void
 *
 *  \return uint8_t data, data to be read.
 */

uint8_t SPI_read()
{
	/* Start transmission */
	SPDR = 0x00;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}



/*! \brief  SPI Initialize.
 *  Initializes the SPI, sets MOSI and SCK output. Set respective inputs.
 *
 *  \param void
 *
 *  \return void
 */

void SPI_init(){
	// Set MOSI and SCK output, all others input:
	set_bit(DDRB, DDB5);
	set_bit(DDRB, DDB7);
    // Set chip-select:
	set_bit(DDRB, DDB4);
	clear_bit(DDRB,PB6);
	// Enable SPI, Master, set clock rate fck/16:
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}



/*! \brief  SPI Chipselect.
 *  Chipselect for the SPI.
 *
 *  \param uint8_t select, true or false for selection.
 *
 *  \return void
 */

void SPI_chipSelect(uint8_t select){
	
	if(select == 0){
		clear_bit(PORTB, PB4);
	}
	
	else{
		set_bit(PORTB, PB4);
	}
}
