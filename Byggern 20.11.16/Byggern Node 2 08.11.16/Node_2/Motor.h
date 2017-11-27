/*
 * IncFile1.h
 *
 * Created: 08.11.2016 11:23:02
 *  Author: tobiasu
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "joystick.h"
#include "controller.h"
#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit ) (reg &= ~(1 << bit))

#ifndef INCFILE1_H_
#define INCFILE1_H_

#define DIR PH1
#define SEL PH3
#define EN PH4
#define RST PH6
#define OE PH5

#define EASYGAME 0
#define NORMALGAME 1
#define HARDGAME 2

#define NON_INVERTED_MODE 0
#define INVERTED_MODE 1

#define POSITION_REGULATOR 0
#define SPEED_REGULATOR 1

#define EASYSCALE 1.5
#define MEDIUMSCALE 2
#define HARDSCALE 4

void DAC_init();
void MOTOR_init();
int MOTOR_read_pos();
void encoderReset();
void set_regulator(CAN_message msg);
uint8_t get_regulator();
void MOTOR_controller_init();
void MOTOR_set_speed_regulator(Joy_pos joystick_data, struct PI *pi);
void MOTOR_set_position_regulator(uint8_t slider, struct PI *pi);
void MOTOR_controller(CAN_message msg, struct PI *pi);
void set_difficulty(CAN_message msg, struct PI *pi);
void set_inverted_mode(CAN_message msg);
#endif /* INCFILE1_H_ */