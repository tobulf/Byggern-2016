/*
 * OLEDdriver.c
 *
 * Created: 20.09.2016 14:26:38
 *  Author: bragesae
 */ 
#include "OLEDdriver.h"
#include "font8x8.h"
#include <stdio.h>
#include "oled_drawings.h"
#include "menu.h"
#include "util/delay.h"
volatile uint8_t *oled_cmd = (uint8_t  *) 0x1000;
volatile uint8_t *oled_data = (uint8_t *) 0x1200;

#define set_bit(reg, bit ) (reg |= (1 << bit))

uint8_t col, page;

void OLED_write_command(uint8_t address){
	*oled_cmd = address;
}

void OLED_write_data(uint8_t data)
{
	*oled_data = data;
}

void OLED_init()
{
	
	OLED_write_command(0xae); // display off
	OLED_write_command(0xa1); //segment remap
	OLED_write_command(0xda); //common pads hardware: alternative
	OLED_write_command(0x12);
	OLED_write_command(0xc8); //common output scan direction:com63~com0
	OLED_write_command(0xa8); //multiplex ration mode:63
	OLED_write_command(0x3f);
	OLED_write_command(0xd5); //display divide ratio/osc. freq. mode
	OLED_write_command(0x80);
	OLED_write_command(0x81); //contrast control
	OLED_write_command(0x50);
	OLED_write_command(0xd9); //set pre-charge period
	OLED_write_command(0x21);
	OLED_write_command(0x20); //Set Memory Addressing Mode
	OLED_write_command(0x02);
	OLED_write_command(0xdb); //VCOM deselect level mode
	OLED_write_command(0x30);
	OLED_write_command(0xad); //master configuration
	OLED_write_command(0x00);
	OLED_write_command(0xa4); //out follows RAM content
	OLED_write_command(0xa6); //set normal display
	OLED_write_command(0xaf); // display on
	
	OLED_reset();
	
	
}



void OLED_home(){
	col = 0;
	page = 0;
	/*
	
	//set start col = 0
	OLED_write_command(0x21);
	OLED_write_command(0x0);
	OLED_write_command(0x7f);
	
	//set start page = 0
	OLED_write_command(0xB0);

	*/
	OLED_go_to_pos(0,0);
	
}

void OLED_reset(){
	OLED_go_to_pos(0,0);
	
	for (int i = 0; i < 8; i++){
		OLED_goto_page(i);
		
		for (int j = 0; j < 128; j++){
			OLED_write_data(0x00);
			
		}
	}
	OLED_go_to_pos(0,0);
	
	
	
}



void OLED_clear_char(){
	for (int i = 0; i < FONTSIZE; i++){
		OLED_write_data(0x00);
	}
}

int OLED_print_char(char input){
	for (int i = 0; i < FONTSIZE; i++){
		OLED_write_data(pgm_read_byte(&font[(int)input-32][i]));
		col ++;
	}
	return 0;
}

int print_char(char input,FILE*stream){
		for (int i = 0; i < FONTSIZE; i++){
			OLED_write_data(pgm_read_byte(&font[(int)input-32][i]));
			col ++;
		}
		return 0;
}

void OLED_print_sentence(char sentence[]){
	for (int i = 0; i < strlen(sentence); i++){
		if (col > 128 - FONTSIZE+1){
			OLED_goto_page(page+1);
			
			OLED_goto_column(0);
			
			if (page == 8){
				OLED_home();
				break;
			}
		}
		
		OLED_print_char(sentence[i]);
		
	} 
}

void OLED_goto_page(uint8_t newpage){
	page = newpage;
	OLED_write_command(0xB0+page);
}

void OLED_goto_column(uint8_t column){
 	col = column;
 	if (column < 16){
 		OLED_write_command(0x00+column);
		OLED_write_command(0x10);
 		
 	}
	else{
		OLED_write_command((column & 0b00001111));
		OLED_write_command(0x10+((column)>>4));
	}
 
 }
 
void OLED_go_to_pos(uint8_t page, uint8_t column){
	 OLED_goto_column(column);
	 OLED_goto_page(page);
 }

void OLED_print_figure_8x8(int columntest){
	OLED_reset();
	OLED_goto_column(columntest);
	
 	for (uint8_t k = 0; k < 64; k+=8){

		 	for (uint8_t j = 0; j < 8; j++){
			 	for (uint8_t i = 0; i < FONTSIZE; i++){
				 	OLED_write_data(pgm_read_byte(&egentegning[j+k][i]));
			 	}
		 	}
			OLED_goto_page(page+1);
			OLED_goto_column(columntest);
 
	 }
	 
	 OLED_home();
	 


	
}

void OLED_credits(){
	OLED_print_credits();
	_delay_ms(10000);
	OLED_print_henning();
	_delay_ms(10000);
	OLED_print_brage();
	_delay_ms(10000);
	OLED_print_tobias();
	_delay_ms(10000);
	
	
}



void OLED_print_brage(){
	OLED_reset();
	
	OLED_goto_column(0);
	for (uint8_t k = 0; k < 128; k+=16){

		for (uint8_t j = 0; j < 16; j++){
			for (uint8_t i = 0; i < FONTSIZE; i++){
				OLED_write_data(pgm_read_byte(&brage[j+k][i]));
			}
		}
		OLED_goto_page(page+1);
		OLED_goto_column(0);
	}
	OLED_home();	
	OLED_print_sentence("Brage");
}

void OLED_print_tobias(){
	OLED_reset();
	
	OLED_goto_column(0);
	for (uint8_t k = 0; k < 128; k+=16){

		for (uint8_t j = 0; j < 16; j++){
			for (uint8_t i = 0; i < FONTSIZE; i++){
				OLED_write_data(pgm_read_byte(&tobias[j+k][i]));
			}
		}
		OLED_goto_page(page+1);
		OLED_goto_column(0);
	}
	OLED_home();
	OLED_print_sentence("Tobias");
}

void OLED_print_henning(){
	OLED_reset();
	
	OLED_goto_column(0);
	for (uint8_t k = 0; k < 128; k+=16){

		for (uint8_t j = 0; j < 16; j++){
			for (uint8_t i = 0; i < FONTSIZE; i++){
				OLED_write_data(pgm_read_byte(&henning[j+k][i]));
			}
		}
		OLED_goto_page(page+1);
		OLED_goto_column(0);
	}
	OLED_home();
	OLED_print_sentence("Henning");
}

void OLED_print_credits(){
	OLED_reset();
	OLED_goto_column(0);
	for (uint8_t k = 0; k < 128; k+=16){

		for (uint8_t j = 0; j < 16; j++){
			for (uint8_t i = 0; i < FONTSIZE; i++){
				OLED_write_data(pgm_read_byte(&credits[j+k][i]));
			}
		}
		OLED_goto_page(page+1);
		OLED_goto_column(0);
	}
	OLED_home();
}
