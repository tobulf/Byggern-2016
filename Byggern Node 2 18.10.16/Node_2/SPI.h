/*
 * SPI.h
 *
 * Created: 04.10.2016 14:33:01
 *  Author: tobiasu
 */ 
#include <avr/io.h>
#include <stdio.h>

#ifndef SPI_H_
#define SPI_H_

void SPI_write(uint8_t data);
uint8_t SPI_read();
void SPI_init();
void SPI_chipSelect(uint8_t select);



#endif /* SPI_H_ */