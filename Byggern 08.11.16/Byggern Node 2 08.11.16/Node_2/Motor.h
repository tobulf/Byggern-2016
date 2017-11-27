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





void DAC_init();
void MOTOR_init();
void MOTOR_set_speed(Joy_pos joystick_data, struct PI pi);
int MOTOR_read_pos();
void encoderReset();
#endif /* INCFILE1_H_ */