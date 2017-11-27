/*
 * MCP.c
 *
 * Created: 04.10.2016 16:40:29
 *  Author: tobiasu
 */ 


#include "MCPdriver.h"
#include "MCP2515.h"

#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit ) (reg &= ~(1 << bit))
#define test_bit(reg, bit ) (reg & (1 << bit))



uint8_t mcp2515_read(uint8_t address)
{
	
	clear_bit(PINB, PB4);
	SPI_transmit(MCP_READ);
	SPI_transmit(address);
	uint8_t result = SPI_transmit(0x00);
	set_bit(PINB, PB4);
	return result;
	
	
	/*
	uint8_t result;
	PORTB &= ~(1<<CAN_CS); // Select CAN-controller
	SPI_write(MCP_READ); // Send read command
	SPI_write(address); // Send address
	result = SPI_transmit(); // Read result
	PORTB |= (1<<CAN_CS); // Deselect CAN-controller
	return result;
	*/
}


/*

void SPI_SlaveInit(void)
{
	//Set MISO output, all others input 
	set_bit(DDRB, PB6);
	DDR_SPI = (1<<DD_MISO);
	// Enable SPI 
	SPCR = (1<<SPE);
}

uint8_t mcp2515_init()
{
	uint8_t value;
	SPI_SlaveInit(); // Initialize SPI
	mcp2515_reset(); // Send reset-command
	// Self-test
	mcp2515_read(MCP_CANSTAT, &value);
	if ((value & MODE_MASK) != MODE_CONFIG) {
		printf(”MCP2515 is NOT in configuration mode
		after reset!\n”);
		return 1;
	}
	// More initialization
	return 0;
}


*/