/*
 * buzzer.c
 *
 * Created: 18.11.2016 16:27:26
 *  Author: bragesae
 */ 


#include "buzzer.h"
#include <avr/io.h>
#include <stdbool.h>
#include <stdio.h>
#define F_CPU 16000000
#include <util/delay.h>
#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit ) (reg &= ~(1 << bit))


#define melodyPin 3
//Mario main theme melody

int starwars[] = {
	NOTE_C4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C5, 
	NOTE_G4, NOTE_F4, NOTE_D4, NOTE_C5, NOTE_G4, NOTE_F4,
	NOTE_E4, NOTE_F4, NOTE_D4
};

int starwars_tempo[]={
	50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50
};

int starwars_duration[] = {
	1000, 1000, 250, 250, 250, 1000, 500, 250, 250, 250, 1000, 500, 250
};

int melody[] = {
	NOTE_E7, NOTE_E7, 0, NOTE_E7,
	0, NOTE_C7, NOTE_E7, 0,
	NOTE_G7, 0, 0,  0,
	NOTE_G6, 0, 0, 0,
	
	NOTE_C7, 0, 0, NOTE_G6,
	0, 0, NOTE_E6, 0,
	0, NOTE_A6, 0, NOTE_B6,
	0, NOTE_AS6, NOTE_A6, 0,
	
	NOTE_G6, NOTE_E7, NOTE_G7,
	NOTE_A7, 0, NOTE_F7, NOTE_G7,
	0, NOTE_E7, 0, NOTE_C7,
	NOTE_D7, NOTE_B6, 0, 0,
	
	NOTE_C7, 0, 0, NOTE_G6,
	0, 0, NOTE_E6, 0,
	0, NOTE_A6, 0, NOTE_B6,
	0, NOTE_AS6, NOTE_A6, 0,
	
	NOTE_G6, NOTE_E7, NOTE_G7,
	NOTE_A7, 0, NOTE_F7, NOTE_G7,
	0, NOTE_E7, 0, NOTE_C7,
	NOTE_D7, NOTE_B6, 0, 0
};
//Mario main them tempo
int tempo[] = {
	12, 12, 12, 12,
	12, 12, 12, 12,
	12, 12, 12, 12,
	12, 12, 12, 12,
	
	12, 12, 12, 12,
	12, 12, 12, 12,
	12, 12, 12, 12,
	12, 12, 12, 12,
	
	9, 9, 9,
	12, 12, 12, 12,
	12, 12, 12, 12,
	12, 12, 12, 12,
	
	12, 12, 12, 12,
	12, 12, 12, 12,
	12, 12, 12, 12,
	12, 12, 12, 12,
	
	9, 9, 9,
	12, 12, 12, 12,
	12, 12, 12, 12,
	12, 12, 12, 12,
};
//Underworld melody
int underworld_melody[] = {
	NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
	NOTE_AS3, NOTE_AS4, 0,
	0,
	NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
	NOTE_AS3, NOTE_AS4, 0,
	0,
	NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
	NOTE_DS3, NOTE_DS4, 0,
	0,
	NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
	NOTE_DS3, NOTE_DS4, 0,
	0, NOTE_DS4, NOTE_CS4, NOTE_D4,
	NOTE_CS4, NOTE_DS4,
	NOTE_DS4, NOTE_GS3,
	NOTE_G3, NOTE_CS4,
	NOTE_C4, NOTE_FS4, NOTE_F4, NOTE_E3, NOTE_AS4, NOTE_A4,
	NOTE_GS4, NOTE_DS4, NOTE_B3,
	NOTE_AS3, NOTE_A3, NOTE_GS3,
	0, 0, 0
};
//Underwolrd tempo
int underworld_tempo[] = {
	12, 12, 12, 12,
	12, 12, 6,
	3,
	12, 12, 12, 12,
	12, 12, 6,
	3,
	12, 12, 12, 12,
	12, 12, 6,
	3,
	12, 12, 12, 12,
	12, 12, 6,
	6, 18, 18, 18,
	6, 6,
	6, 6,
	6, 6,
	18, 18, 18, 18, 18, 18,
	10, 10, 10,
	10, 10, 10,
	3, 3, 3
};



int song = 0;


void buzzer_init(){
	set_bit(DDRF, PF2);
}

void sing(int s) {
	// iterate over the notes of the melody:
	int song = s;
	if (song == 2) {
		int size = sizeof(underworld_melody) / sizeof(int);
		for (int thisNote = 0; thisNote < size; thisNote++) {
			
			// to calculate the note duration, take one second
			// divided by the note type.
			//e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
			int noteDuration = 1000 / underworld_tempo[thisNote];
			
			buzz(underworld_melody[thisNote], noteDuration);
			
			// to distinguish the notes, set a minimum time between them.
			// the note's duration + 30% seems to work well:
			int pauseBetweenNotes = noteDuration * 1.30;
			super_delay_ms(pauseBetweenNotes);
			
			// stop the tone playing:
			buzz(0, noteDuration);
			
		}
		
		} else if (song == 1) {
		int size = sizeof(melody) / sizeof(int);
		for (int thisNote = 0; thisNote < size; thisNote++) {
			
			// to calculate the note duration, take one second
			// divided by the note type.
			//e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
			int noteDuration = 1000 / tempo[thisNote];
			
			buzz(melody[thisNote], noteDuration);
			
			// to distinguish the notes, set a minimum time between them.
			// the note's duration + 30% seems to work well:
			int pauseBetweenNotes = noteDuration * 1.30;
			super_delay_ms(pauseBetweenNotes);
			
			// stop the tone playing:
			buzz(0, noteDuration);
			
		}
	} else if(song == 3){
		int size = sizeof(starwars) / sizeof(int);
		for (int thisNote = 0; thisNote < size; thisNote++) {
			
			// to calculate the note duration, take one second
			// divided by the note type.
			//e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
			int noteDuration = starwars_duration[thisNote];
			
			buzz(starwars[thisNote], noteDuration);
			
			// to distinguish the notes, set a minimum time between them.
			// the note's duration + 30% seems to work well:
			int pauseBetweenNotes = noteDuration * 1.30;
			super_delay_ms(starwars_tempo[thisNote]);
			
			// stop the tone playing:
			buzz(0, noteDuration);
	}
}
		}

void buzz(long frequency, long length) {
	long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
	//// 1 second's worth of microseconds, divided by the frequency, then split in half since
	//// there are two phases to each cycle
	long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
	//// multiply frequency, which is really cycles per second, by the number of seconds to
	//// get the total number of cycles to produce
	int intdelay = ((int) (delayValue));
	for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
		set_bit(PORTF, PD2); // write the buzzer pin high to push out the diaphram
		super_delay_us(intdelay); // wait for the calculated delay value
		clear_bit(PORTF, PD2); // write the buzzer pin low to pull back the diaphram
		super_delay_us(intdelay); // wait again or the calculated delay value
	}
}

void super_delay_ms(unsigned int milliseconds)
{
	while(milliseconds > 0)
	{
		_delay_ms(1);
		milliseconds--;
	}
}

void super_delay_us(unsigned int microseconds)
{
	while(microseconds > 0)
	{
		_delay_us(1);
		microseconds--;
	}
}