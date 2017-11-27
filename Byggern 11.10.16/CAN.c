/*
 * CAN.c
 *
 * Created: 04.10.2016 14:32:30
 *  Author: tobiasu
 */ 
#include "CAN.h"
#include "MCP2515.h"
#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit ) (reg &= ~(1 << bit))





CAN_init(){
	MCP2515_init();
	MCP2515_bitModify(MCP_CANCTRL, 0b11100000, MODE_LOOPBACK); 
}

// CAN_transmit(){
// 	MCP2515_requestToSend(MCP_RTS_TX0);
// 	
// }

void CAN_message_send(CAN_message* message) {
	MCP2515_bitModify(MCP_TXB0SIDL, 0b00010000, 0b00001000);
	MCP2515_write(MCP_TXB0SIDH, message->id);
	MCP2515_write(MCP_TXB0SIDL, 0x00);
	MCP2515_write(MCP_TXB0DLC, message->length);
	
	for (uint8_t i = 0; i < message->length; i++){
		MCP2515_write(MCP_TXB0D0 + i, message->data[i]);	
	}
	MCP2515_requestToSend(MCP_RTS_TX0);
	
}

void CAN_message_recieve(CAN_message *msg){
	msg->id= MCP2515_read(MCP_RXB0SIDH);
	msg->length=MCP2515_read(MCP_RXB0DLC);
	for(uint8_t i =0; i < msg->length;i++){
		msg->data[i]=MCP2515_read(MCP_RXB0D0 + i);
	}
}
