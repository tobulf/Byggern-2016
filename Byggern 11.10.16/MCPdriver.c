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

#define chipLOW 0
#define chipHIGH 1


uint8_t MCP2515_init(){
	SPI_init();
	MCP2515_reset();
	//uint8_t value;
	//MCP2515_read(MCP_CANSTAT, &value);
	//if ((value & MODE_MASK) != MODE_CONFIG) {
	//	printf("MCP2515 is NOT in configuration mode after reset!\n");
	//	return 1;
	//}
	//else{
	//	return 0;
	//}
}



uint8_t MCP2515_read(uint8_t address)
{
	
	SPI_chipSelect(chipLOW);
	SPI_write(MCP_READ);
	SPI_write(address);
	uint8_t result = SPI_read();
	SPI_chipSelect(chipHIGH);
	return result;
	
}

void MCP2515_write(uint8_t address, uint8_t data){
	SPI_chipSelect(chipLOW);
	SPI_write(MCP_WRITE);
	SPI_write(address);
	SPI_write(data);
	SPI_chipSelect(chipHIGH);
}


void MCP2515_requestToSend(uint8_t address){
	
	SPI_chipSelect(chipLOW);
	if(address == MCP_RTS_TX0 || address == MCP_RTS_TX1 || address == MCP_RTS_TX2 || address == MCP_RTS_ALL){
		SPI_write(address);	
	}
	
	else{
		OLED_print_pig();
		_delay_ms(100000);
	}
	SPI_chipSelect(chipHIGH);
}

uint8_t MCP2515_readStatus(){
	SPI_chipSelect(chipLOW);
	SPI_write(MCP_READ_STATUS);
	uint8_t readStatus = MCP2515_read(SPDR);
	SPI_chipSelect(chipHIGH);
	return readStatus;
}

void MCP2515_bitModify(uint8_t address ,uint8_t maskByte, uint8_t data){
	SPI_chipSelect(chipLOW);
	SPI_write(MCP_BITMOD);
	SPI_write(address);
	SPI_write(maskByte);
	SPI_write(data);
	SPI_chipSelect(chipHIGH);
}

void MCP2515_reset(){
	SPI_chipSelect(chipLOW);
	SPI_write(MCP_RESET);
	SPI_chipSelect(chipHIGH);
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