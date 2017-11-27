/*
 * MCP.c
 *
 * Created: 04.10.2016 16:40:29
 *  Author: tobiasu
 */

/*! \file *********************************************************************
 *
 * \brief MCP2515 DRIVER.
 *
 *  Initialises MCP2515, Also function for Writing, reading, request to send, read status, bit modify and reset.
 *
 *
 *
 *****************************************************************************/




#include "MCPdriver.h"
#include "SPI.h"

// Macros for command-adresses to the SPI:
#include "MCP2515.h"

// Macros:
#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit ) (reg &= ~(1 << bit))
#define test_bit(reg, bit ) (reg & (1 << bit))

// Defining chipselect high and low.
#define CHIPLOW 0
#define CHIPHIGH 1





/*! \brief  MCP2515 initialize.
 *  Initializes SPI, and resets MCP2515.
 *
 *  \param void
 *
 *  \return void
 */

uint8_t MCP2515_init(){
	SPI_init();
	MCP2515_reset();
}



/*! \brief  MCP2515 read.
 *  Reads data on SPI.
 *
 *  \param uint8_t address, command to SPI.
 *
 *  \return uint8_t result, returns contents of SPI register.
 */

uint8_t MCP2515_read(uint8_t address){
	SPI_chipSelect(CHIPLOW);
    // Write command to SPI:
	SPI_write(MCP_READ);
    // Read adress:
	SPI_write(address);
    // Get adress-data from SPI:
	uint8_t result = SPI_read();
	SPI_chipSelect(CHIPHIGH);
	return result;
	
}



/*! \brief  MCP2515 write.
 *  Sends data on SPI.
 *
 *  \param uint8_t address, SPI command.
 *  \param uint8_t data, Data to be sent.
 *
 *  \return void
 */

void MCP2515_write(uint8_t address, uint8_t data){
	SPI_chipSelect(CHIPLOW);
    // Write command to SPI:
	SPI_write(MCP_WRITE);
    // Select Adress to write to:
	SPI_write(address);
    // Write data to adress:
	SPI_write(data);
	SPI_chipSelect(CHIPHIGH);
}



/*! \brief  MCP2515 request to send.
 *  Sends a request to send on SPI.
 *
 *  \param uint8_t adress, command to SPI
 *
 *  \return void
 */

void MCP2515_requestToSend(uint8_t address){
	SPI_chipSelect(CHIPLOW);
	if(address == MCP_RTS_TX0 || address == MCP_RTS_TX1 || address == MCP_RTS_TX2 || address == MCP_RTS_ALL){
		SPI_write(address);	
	}
	
	else{
        // SPI write error prints pig on OLED:
		OLED_print_pig();
		_delay_ms(100000);
	}
	SPI_chipSelect(CHIPHIGH);
}



/*! \brief  MCP2515 readstatus.
 *  Read status of MCP2515, read status of SPI.
 *
 *  \param void
 *
 *  \return uint8_t readStatus, Returns status. 
 */

uint8_t MCP2515_readStatus(){
	SPI_chipSelect(CHIPLOW);
    // Write command to SPI:
	SPI_write(MCP_READ_STATUS);
    // Read status from SPI:
	uint8_t readStatus = MCP2515_read(SPDR);
	SPI_chipSelect(CHIPHIGH);
	return readStatus;
}



/*! \brief  MCP2515 Bitmodify.
 *  function to modify/mask bit in SPI.
 *
 *  \param uint8_t address,
 *  \param uint8_t maskByte,
 *  \param uint8_t data,
 *
 *  \return void
 */

void MCP2515_bitModify(uint8_t address ,uint8_t maskByte, uint8_t data){
	SPI_chipSelect(CHIPLOW);
    // Write command to SPI:
	SPI_write(MCP_BITMOD);
    // Address of data to be masked/modified:
	SPI_write(address);
    // Masks the data:
	SPI_write(maskByte);
    // Writes masked data:
	SPI_write(data);
	SPI_chipSelect(CHIPHIGH);
}



/*! \brief  MCP2515 reset.
 *  function to reset the MCP2515
 *
 *  \param void
 *
 *  \return void
 */

void MCP2515_reset(){
	SPI_chipSelect(CHIPLOW);
    // Write command to SPI:
	SPI_write(MCP_RESET);
	SPI_chipSelect(CHIPHIGH);
}

