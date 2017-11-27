/*
 * solenoid.h
 *
 * Created: 01.11.2016 17:51:43
 *  Author: bragesae
 */ 


#ifndef solenoid_H_
#define solenoid_H_
// Defining Solenoid modes:
#define SOLENOID_SEMI 0
#define SOLENOID_AUTO 1
#define SOLENOID_SEMI_AUTO 2

void solenoid_init();
void trigger_solenoid();


#endif /* solenoid_H_ */
