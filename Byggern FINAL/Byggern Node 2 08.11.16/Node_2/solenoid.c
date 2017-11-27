/*
 * solenoid.c
 *
 * Created: 01.11.2016 17:51:31
 *  Author: bragesae
 */ 

/*! \file *********************************************************************
 *
 * \brief Solenoid DRIVER.
 *
 *  Initialises solenoid, functions for setting solenoid-mode and triggering solenoid.
 *
 *
 *
 *****************************************************************************/

// Defining CPU clock speed:
#define F_CPU 16000000

#include <avr/io.h>
#include "CAN.h"
#include "solenoid.h"
#include "util/delay.h"
#include "joystick.h"




// Static variables for solenoid mode:
volatile uint8_t SOLENOID_MODE = SOLENOID_SEMI_AUTO;
//solenoid triggermode = semiautomatic:
volatile uint8_t SOLENOID_BUTTON_PRESSED = 0; 
//solenoid triggermode = automatic:
volatile uint8_t SOLENOID_COUNTER = 0; 

// Macros:
#define test_bit(reg, bit ) (reg & (1 << bit))
#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit ) (reg &= ~(1 << bit))


/*! \brief  solenoid initialize.
 *  Initializes solenoid, sets PF1 to output.
 *
 *  \param void
 *
 *  \return void
 */

void solenoid_init(){
	set_bit(DDRF,PF1);
}



/*! \brief  Set solenoid mode.
 *  Set solenoid fire-mode.
 *
 *  \param Can_message msg, message containing mode.
 *
 *  \return void
 */

void set_solenoid_mode(CAN_message msg){
	if (msg.data[2] == SOLENOID_AUTO){
		SOLENOID_MODE = SOLENOID_AUTO;
	}
	if (msg.data[2] == SOLENOID_SEMI){
		SOLENOID_MODE = SOLENOID_SEMI;
	}
	if (msg.data[2] == SOLENOID_SEMI_AUTO){
		SOLENOID_MODE = SOLENOID_SEMI_AUTO;
	}
}




/*! \brief  Trigger solenoid.
 *  Triggers solenoid.
 *
 *  \param void
 *
 *  \return void
 */

void trigger_solenoid(){
	set_bit(PORTF,PF1);
	// Delay to prevent further disturbances:
	_delay_ms(35);
	clear_bit(PORTF,PF1);
	_delay_ms(5);
}



/*! \brief  Trigger solenoid mode.
 *  Triggers solenoid, in different modes.
 *
 *  \param Can_message msg, message containing button data.
 *
 *  \return void
 */

void trigger_solenoid_mode(CAN_message msg){
	if (SOLENOID_MODE == SOLENOID_SEMI){
		if ((msg.data[RIGHT_BUTTON] == 1)  && (SOLENOID_BUTTON_PRESSED == 0)){
			trigger_solenoid();
			SOLENOID_BUTTON_PRESSED = 1;
		}
		else if ((msg.data[RIGHT_BUTTON] == 0)  && (SOLENOID_BUTTON_PRESSED == 1)){
			SOLENOID_BUTTON_PRESSED = 0;
		}
	}
	if (SOLENOID_MODE == SOLENOID_SEMI_AUTO){
		SOLENOID_COUNTER++;
		if (msg.data[RIGHT_BUTTON] == 1){
			if (SOLENOID_COUNTER > 2){
				trigger_solenoid();
				SOLENOID_COUNTER = 0;
			}
			
			
		}
	}
	if (SOLENOID_MODE == SOLENOID_AUTO){
		SOLENOID_COUNTER++;
		if (SOLENOID_COUNTER > 5){
			SOLENOID_COUNTER = 0;
			trigger_solenoid();
		}
	}
}