/*
 * OLEDdriver.h
 *
 * Created: 20.09.2016 14:26:52
 *  Author: bragesae
 */ 


#ifndef OLEDDRIVER_H_
#define OLEDDRIVER_H_

#include <stdint.h>
#include <stdio.h>
void OLED_init();
void OLED_write_command(uint8_t address);
void OLED_write_data(uint8_t data);
void OLED_home();
void OLED_reset();
void OLED_goto_page(uint8_t page);
void OLED_goto_column(uint8_t column);
void OLED_go_to_pos(uint8_t page, uint8_t column);
int print_char(char input,FILE*stream);
int OLED_print_char(char input);
void OLED_fill();
void OLED_clear_char();
void OLED_print_brage();
void OLED_print_tobias();
void OLED_print_henning();
void OLED_print_credits();
void OLED_credits();

void OLED_print_sentence(char sentence[]);
void OLED_print_figure_8x8(int columntest);
void dummy_func();

static FILE FONT5 = FDEV_SETUP_STREAM(print_char,NULL,_FDEV_SETUP_WRITE);
#define FONT (&FONT5)

#endif /* OLEDDRIVER_H_ */