/*  Partner(s) Name & E-mail: Ji Hoon Choi (jchoi071@ucr.edu), Ji Houn Huh (jhuh009@ucr.edu)
 *  Lab Section: 23
 *  Assignment: Lab # 6 Exercise # 1
 *  Exercise Description: 3 Blinking Sync LEDS
 *  
 *  I acknowledge all content contained herein, excluding template or example
 *  code, is my own original work.
 */

#include <avr/io.h>
#include "timer.h"

enum States { Start, S0, S1, S2} state;
unsigned char tmpC = 0x00;

void Tick() {
	switch(state) {
		case Start: state = S0; break;
		case S0: state = S1; break;
		case S1: state = S2; break;
		case S2: state = S0; break;
		default: state = Start; break;
	}
	switch(state) {
		case Start: break;
		case S0: tmpC = 0x01; PORTC = tmpC; break;
		case S1: tmpC = 0x02; PORTC = tmpC; break;
		case S2: tmpC = 0x04; PORTC = tmpC; break;
		default: break;
	}
}


int main(void)
{
	DDRC = 0xFF;
	PORTC = 0x00;
	TimerSet(1000);
	TimerOn();
	//unsigned char tmpC = 0x00;
    /* Replace with your application code */
    while (1) 
    {
		//tmpC = ~tmpC;
		Tick();
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

