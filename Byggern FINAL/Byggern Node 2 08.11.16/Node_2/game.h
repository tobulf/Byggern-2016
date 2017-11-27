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
#define LSB 0
#define MSB 1


void GAME_send_score(int score);
void GAME_send_gameover(int score);






#endif /* GAME_H_ */
