/*
 * game.h
 *
 * Created: 15.11.2016 18:06:12
 *  Author: bragesae
 */ 
#include <stdbool.h>
#include <avr/io.h>
#include "CAN.h"
#include "menu.h"
#ifndef GAME_H_
#define GAME_H_


//GAME SCORE INDEX
#define LSB 0
#define MSB 1


void start_game(uint8_t arr[]);
void CAN_send_game_settings(game_settings settings);
void GAME_print_score(CAN_message msg);
void GAME_print_finalscore(CAN_message msg);
void GAME_print_highscores();
#endif /* GAME_H_ */
