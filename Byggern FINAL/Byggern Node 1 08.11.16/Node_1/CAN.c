/*
 * CAN.c
 *
 * Created: 04.10.2016 14:32:30
 *  Author: tobiasu
 */

/*! \file *********************************************************************
 *
 * \brief CAN Driver.
 *
 *  Initialises CAN, Also function for Sending And recieving CAN-messages.
 *  Functionality for checking if NODE1 can send and recieve.
 *
 *
 *
 *****************************************************************************/


#include "CAN.h"
#include "MCPdriver.h"

// Macros for command-adresses to the SPI:
#include "MCP2515.h"

// Macros:
#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit ) (reg &= ~(1 << bit))




/*! \brief  CAN initialize.
 *  Initializing CAN.
 *
 *  \param void
 *
 *  \return void
 */

CAN_init(){
	MCP2515_init();
    // Set CAN-mode:
	MCP2515_bitModify(MCP_CANCTRL, 0b11100000, MODE_NORMAL); 
}





/*! \brief  CAN Message send.
 *  Function for sending a CAN message.
 *
 *  \param CAN_message* message, message to be sent.
 *
 *  \return void
 */

void CAN_message_send(CAN_message* message) {
    // Check availability to send:
	Can_CAN_send();
    // Send empty first byte:
	MCP2515_write(MCP_TXB0SIDL, 0b00000000);
    // Send message ID:
	MCP2515_write(MCP_TXB0SIDH, message->id);
    // Send message length:
	MCP2515_write(MCP_TXB0DLC, message->length);
	// Send an array of given length:
	for (uint8_t i = 0; i < message->length; i++){
		MCP2515_write(MCP_TXB0D0 + i, message->data[i]);	
	}
    // Send message:
	MCP2515_requestToSend(MCP_RTS_TX0);
}





/*! \brief  CAN message recieve .
 *  Command for recieving message from CAN.
 *
 *  \param void CAN_message *msg, takes in a pointer which is modified to become the recieved message.
 *
 *  \return void
 */

void CAN_message_recieve(CAN_message *msg){
	// Check availability for reception:
	Can_CAN_receive();
	
	volatile uint8_t intf = MCP2515_read(CANINTF);
	// Reads CAN-message ID:
	msg->id= MCP2515_read(MCP_RXB0SIDH);
	// Read CAN-message length:
	msg->length=MCP2515_read(MCP_RXB0DLC);
	// Read CAN-message data:
	for(uint8_t i =0; i < msg->length;i++){
		msg->data[i]=MCP2515_read(MCP_RXB0D0 + i);
	}
	
	MCP2515_bitModify(CANINTF, 0b00000001, 0);
}





/*! \brief  can CAN send.
 *  Checking if the CAN is ready to send.
 *
 *  \param void
 *
 *  \return void
 */

void Can_CAN_receive(){
	uint8_t intf = MCP2515_read(CANINTF);
    // Polling until ready:
	while ((intf & 0b00000011) == 0) {
		intf = MCP2515_read(CANINTF);
	}
}





/*! \brief  can CAN recieve.
 *  Checking if the CAN is ready to recieve.
 *
 *  \param void
 *
 *  \return void
 */

void Can_CAN_send(){
	volatile uint8_t intf = MCP2515_read(TXB0CTRL);
    // Polling until ready:
	while ((intf & 0b00001000) != 0) {
		intf = MCP2515_read(TXB0CTRL);
	}
}

