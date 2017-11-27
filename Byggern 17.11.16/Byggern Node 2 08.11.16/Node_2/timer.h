/*
 * IncFile1.h
 *
 * Created: 25.10.2016 13:55:17
 *  Author: tobiasu
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_

void timer_init(uint8_t* flagPtr);
double get_wall_time(void);
void timer_start();
void score_timer();
int score_get_time();
void score_reset_time();
void PI_timer(uint8_t *PI_flagPtr);
int score_get_score();
#endif /* INCFILE1_H_ */