/*
 * game.h
 *
 * Created: 15.11.2016 16:19:27
 *  Author: bragesae
 */ 

#include <avr/io.h>
#include <stdio.h>
#ifndef GAME_H_
#define GAME_H_




void GAME_send_score(uint8_t score);
void GAME_send_gameover(uint8_t score);





#endif /* GAME_H_ */
