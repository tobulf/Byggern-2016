/*
 * OLEDdriver.c
 *
 * Created: 20.09.2016 14:26:38
 *  Author: bragesae
 */

/*! \file *********************************************************************
 *
 * \brief IO board driver.
 *
 *  Initialises joystick, Also function for calibration, extracting position of sliders and joystick.
 *  Also function for sending all user input over CAN.
 *
 *
 *
 *****************************************************************************/

#include "OLEDdriver.h"
#include "font8x8.h"
#include <stdio.h>
#include "oled_drawings.h"
#include "menu.h"
#include "util/delay.h"
#include <stdlib.h>

// Memory location for OLED:
volatile uint8_t *oled_cmd = (uint8_t  *) 0x1000;
volatile uint8_t *oled_data = (uint8_t *) 0x1200;


// Macro for setting bit:
#define set_bit(reg, bit ) (reg |= (1 << bit))

// Variables for current column and page:
uint8_t col, page; 




/*! \brief  Write command OLED display.
 *  Writes command to OLED.
 *
 *  \param uint8_t adress, command adress.
 *
 *  \return void
 */

void OLED_write_command(uint8_t address){
	*oled_cmd = address;
}




/*! \brief Write data to OLED.
 *  Writes and stores data to OLED register.
 *
 *  \param uint8_t data, data to be stored in register.
 *
 *  \return void
 */

void OLED_write_data(uint8_t data){
	*oled_data = data;
}




/*! \brief  Initialize OLED.
 *  Writes initializing commands to OLED.
 *
 *  \param void
 *
 *  \return void
 */

void OLED_init(){
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
	OLED_reset();             // reset
}



/*! \brief OLED home.
 *  Goes to the first page and first column.
 *
 *  \param void
 *
 *  \return void
 */

void OLED_home(){
	OLED_go_to_pos(0,0);
}



/*! \brief  Reset OLED.
 *  Empty OLED display, and go to the first page and first column.
 *
 *  \param void
 *
 *  \return void
 */

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




/*! \brief  CLEAR char.
 *  Clears current 8x8 char.
 *
 *  \param void
 *
 *  \return void
 */

void OLED_clear_char(){
	for (int i = 0; i < FONTSIZE; i++){
		OLED_write_data(0x00);
	}
}





/*! \brief  OLED Print char.
 *  Prints a char to current position on OLED display. 
 *
 *  \param char input, char to be written.
 *
 *  \return void
 */

void OLED_print_char(char input){
	for (int i = 0; i < FONTSIZE; i++){
		OLED_write_data(pgm_read_byte(&font[(int)input-32][i]));
		col ++;
	}
}





/*! \brief  Print char.
 *  Prints a char to current position on OLED display, using stream.
 *
 *  \param char input, char to be written.
 *
 *  \return void
 */

void print_char(char input,FILE*stream){
		for (int i = 0; i < FONTSIZE; i++){
			OLED_write_data(pgm_read_byte(&font[(int)input-32][i]));
			col ++;
		}
}





/*! \brief  Print Sentence.
 *  Prints a whole sentence to the OLED.
 *
 *  \param char sentence[], sentence to be written.
 *
 *  \return void
 */

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





/*! \brief  Print int.
 *  Prints an int to the OLED.
 *
 *  \param int value, int to be written.
 *
 *  \return void
 */

void OLED_print_int(int value){
	char charvalue[10];
	sprintf(charvalue, "%d", value);
	OLED_print_sentence(charvalue);
}




/*! \brief  Print score.
 *  Prints an int to the OLED.
 *
 *  \param int score, score to be written.
 *
 *  \return void
 */

void OLED_print_score(int score){
	OLED_go_to_pos(2, 44);
	OLED_print_sentence("score: ");
	
	//clear old score from OLED
	OLED_go_to_pos(3, 44);
	for (int i = 0; i < 5; i++){
		OLED_clear_char();
	}
	OLED_go_to_pos(3, 44);
	OLED_print_int(score);
}






/*! \brief  OLED go to page.
 *  goes to newpage.
 *
 *  \param uint8_t newpage, desired page.
 *
 *  \return void
 */

void OLED_goto_page(uint8_t newpage){
	page = newpage;
	OLED_write_command(0xB0+page);
}




/*! \brief  OLED go to column.
 *  goes to column.
 *
 *  \param uint8_t column, desired column.
 *
 *  \return void
 */

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





/*! \brief  OLED go to pos.
 *  goes to position.
 *
 *  \param uint8_t page, desired page.
 *  \uint8_t column, desired column.
 *
 *  \return void
 */

void OLED_go_to_pos(uint8_t page, uint8_t column){
	 OLED_goto_column(column);
	 OLED_goto_page(page);
 }





/*! \brief  OLED credits.
 *  Prints credits to OLED.
 *
 *  \param void
 *
 *  \return void
 */

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


/*! \brief  OLED print Brage.
 *  Prints picture of Brage to OLED.
 *
 *  \param void
 *
 *  \return void
 */
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



/*! \brief  OLED print Tobias.
 *  Prints picture of Tobias to OLED.
 *
 *  \param void
 *
 *  \return void
 */

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





/*! \brief OLED print Henning.
 *  Prints picture of Henning to OLED.
 *
 *  \param void
 *
 *  \return void
 */

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


/*! \brief  OLED print Credits.
 *  Prints credits to OLED.
 *
 *  \param void
 *
 *  \return void
 */

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



/*! \brief OLED print pig.
 *  Prints picture of a pig to OLED.
 *
 *  \param void
 *
 *  \return void
 */

void OLED_print_pig(){
	OLED_reset();
	
	OLED_goto_column(0);
	for (uint8_t k = 0; k < 128; k+=16){

		for (uint8_t j = 0; j < 16; j++){
			for (uint8_t i = 0; i < FONTSIZE; i++){
				OLED_write_data(pgm_read_byte(&pig[j+k][i]));
			}
		}
		OLED_goto_page(page+1);
		OLED_goto_column(0);
	}
	OLED_home();
	OLED_print_sentence("Pig<3");
}
