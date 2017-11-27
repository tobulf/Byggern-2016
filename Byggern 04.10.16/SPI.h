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

uint8_t SPI_transmit(uint8_t data);

void SPI_init();



#endif /* SPI_H_ */