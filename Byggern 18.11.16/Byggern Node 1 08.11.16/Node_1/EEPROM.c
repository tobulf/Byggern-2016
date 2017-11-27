/*
 * EEPROM.c
 *
 * Created: 18.11.2016 18:48:29
 *  Author: bragesae
 */ 
/*! \file *********************************************************************
 *
 * \brief EEPROM Driver.
 *
 *  Contains functions for writing and reading to EEPROM
 *
 *
 *****************************************************************************/

#include "EEPROM.h"
#define F_CPU 4915200
#include <avr/io.h>
#include <stdio.h>


void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE))
	;
	/* Set up address and data registers */
	EEAR = uiAddress;
	EEDR = ucData;
	/* Write logical one to EEMWE */
	EECR |= (1<<EEMWE);
	/* Start eeprom write by setting EEWE */
	EECR |= (1<<EEWE);
}

unsigned char EEPROM_read(unsigned int uiAddress)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE))
	;
	/* Set up address register */
	EEAR = uiAddress;
	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	/* Return data from data register */
	return EEDR;
}

void EEPROM_init(){
	EEPROM_write_int(0, 0);
	EEPROM_write_int(1, 0);
	EEPROM_write_int(2, 0);
	EEPROM_write_int(3, 0);
	EEPROM_write_int(4, 0);
	EEPROM_write_int(50, 0);
	EEPROM_write_int(51, 0);
	EEPROM_write_int(52, 0);
	EEPROM_write_int(53, 0);
	EEPROM_write_int(54, 0);	
}

void EEPROM_write_int(unsigned int address, unsigned int data){
	EEPROM_write(address, data);
	uint8_t msb = (data >> 8);
	EEPROM_write(address+50, msb);
}

unsigned int EEPROM_read_int(unsigned int address){
	int score = (EEPROM_read(address+50) << 8);
	score |= EEPROM_read(address);
	return score;
}

