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





void start_game(uint8_t arr[]);
void CAN_send_game_settings(game_settings settings);

#endif /* GAME_H_ */