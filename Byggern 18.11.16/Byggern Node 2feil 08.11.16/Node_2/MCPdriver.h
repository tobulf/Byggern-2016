/*
 * MCPdriver.h
 *
 * Created: 04.10.2016 16:40:39
 *  Author: tobiasu
 */ 


#include <avr/io.h>
#include <stdio.h>
#include "util/delay.h"

#ifndef MCPDRIVER_H_
#define MCPDRIVER_H_

void SPI_SlaveInit(void);
uint8_t MCP2515_init();
uint8_t MCP2515_read(uint8_t address);
void MCP2515_write(uint8_t address, uint8_t data);
void MCP2515_requestToSend(uint8_t address);
uint8_t MCP2515_readStatus();
void MCP2515_bitModify(uint8_t address ,uint8_t maskByte, uint8_t data);
void MCP2515_reset();

#endif /* MCPDRIVER_H_ */