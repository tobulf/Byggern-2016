/*
 * EEPROM.h
 *
 * Created: 18.11.2016 18:48:20
 *  Author: bragesae
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

void EEPROM_write(unsigned int uiAddress, unsigned char ucData);
unsigned char EEPROM_read(unsigned int uiAddress);
void EEPROM_write_int(unsigned int address, unsigned int data);
unsigned int EEPROM_read_int(unsigned int address);
void EEPROM_init();

#endif /* EEPROM_H_ */
