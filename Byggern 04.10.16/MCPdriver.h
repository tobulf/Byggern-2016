/*
 * MCPdriver.h
 *
 * Created: 04.10.2016 16:40:39
 *  Author: tobiasu
 */ 


#include <avr/io.h>
#include <stdio.h>

#ifndef MCPDRIVER_H_
#define MCPDRIVER_H_

void SPI_SlaveInit(void);
uint8_t mcp2515_init();
uint8_t mcp2515_read(uint8_t address);



#endif /* MCPDRIVER_H_ */