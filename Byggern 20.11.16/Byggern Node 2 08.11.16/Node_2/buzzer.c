/*
 * buzzer.c
 *
 * Created: 18.11.2016 16:27:26
 *  Author: bragesae
 */

/*! \file *********************************************************************
 *
 * \brief Buzzer
 *
 *  Using buzzer to play songs.
 *  BASED ON ARDUINO CODE FOUND ONLINE.
 *
 *
 *
 *****************************************************************************/


#include "buzzer.h"
#include <avr/io.h>
#include <stdbool.h>
#include <stdio.h>
#define F_CPU 16000000
#include <util/delay.h>
#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit ) (reg &= ~(1 << bit))


#define melodyPin 3

// Star Wars melody:
int starwars[] = {
	NOTE_C4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C5, 
	NOTE_G4, NOTE_F4, NOTE_D4, NOTE_C5, NOTE_G4, NOTE_F4,
	NOTE_E4, NOTE_F4, NOTE_D4
};

// Star Wars tempo:
int starwars_tempo[]={
	50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50
};

// Duration of each Star Wars tone:
int starwars_duration[] = {
	1000, 1000, 250, 250, 250, 1000, 500, 250, 250, 250, 1000, 500, 250
};

// Melody for Super Mario main theme:
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

//Mario main theme tempo:
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

//Underworld melody:
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

//Underworld tempo:
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


// current song:
int song = 0;




/*! \brief  buzzer init.
 *  Initilialize the buzzer
 *
 *  \param void
 *
 *  \return void
 */

void buzzer_init(){
	set_bit(DDRF, PF2);
}



/*! \brief  sing.
 *  plays current song
 *
 *  \param s, current song.
 *
 *  \return void
 */

void sing(int s) {
	int song = s;
    
    // Play Underworld melody:
	if (song == 2) {
		int size = sizeof(underworld_melody) / sizeof(int);
        // iterate over the notes of the melody:
		for (int thisNote = 0; thisNote < size; thisNote++) {
			
			// Calculate note duration:
			int noteDuration = 1000 / underworld_tempo[thisNote];
			
            // Start buzzer:
			buzz(underworld_melody[thisNote], noteDuration);
        
			int pauseBetweenNotes = noteDuration * 1.30;
			super_delay_ms(pauseBetweenNotes);
			
			// stop the tone playing:
			buzz(0, noteDuration);
			
		}
		}
    // Play Mario main theme:
    else if (song == 1) {
		int size = sizeof(melody) / sizeof(int);
		for (int thisNote = 0; thisNote < size; thisNote++) {
			
			// Calculate note duration:
			int noteDuration = 1000 / tempo[thisNote];
			
            // Start buzzer:
			buzz(melody[thisNote], noteDuration);
			
			int pauseBetweenNotes = noteDuration * 1.30;
			super_delay_ms(pauseBetweenNotes);
			
			// stop the tone playing:
			buzz(0, noteDuration);
			
		}
    // Play Star Wars theme:
	} else if(song == 3){
		int size = sizeof(starwars) / sizeof(int);
		for (int thisNote = 0; thisNote < size; thisNote++) {
			
			// Calculate note duration:
			int noteDuration = starwars_duration[thisNote];
			
            // Start buzzer:
			buzz(starwars[thisNote], noteDuration);

			super_delay_ms(starwars_tempo[thisNote]);
			
			// stop the tone playing:
			buzz(0, noteDuration);
	}
}
}





/*! \brief  buzz.
 *  Start buzzer.
 *
 *  \param frequency, frequency of  tone.
 *  \param length, length of  tone.
 *
 *  \return void
 */

void buzz(long frequency, long length) {
    // Delay between notes:
	long delayValue = 1000000 / frequency / 2;
    
    // calculate the number of cycles:
	long numCycles = frequency * length / 1000;
	int intdelay = ((int) (delayValue));
	for (long i = 0; i < numCycles; i++) {
        // write the buzzer pin high:
		set_bit(PORTF, PD2);
        // wait for the calculated delay value:
		super_delay_us(intdelay);
        // write the buzzer pin low:
		clear_bit(PORTF, PD2);
        // wait for the calculated delay value:
		super_delay_us(intdelay);
	}
}




/*! \brief  super delay ms.
 *  delay with dynamic delay values.
 *
 *  \param milliseconds, milliseconds to wait.
 *
 *  \return void
 */

void super_delay_ms(unsigned int milliseconds)
{
	while(milliseconds > 0)
	{
		_delay_ms(1);
		milliseconds--;
	}
}






/*! \brief  super delay us.
 *  delay with dynamic delay values.
 *
 *  \param milliseconds, microseconds to wait.
 *
 *  \return void
 */

void super_delay_us(unsigned int microseconds)
{
	while(microseconds > 0)
	{
		_delay_us(1);
		microseconds--;
	}
}
