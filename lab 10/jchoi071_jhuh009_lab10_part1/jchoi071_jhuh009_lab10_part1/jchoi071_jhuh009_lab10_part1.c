/*
 * jchoi071_jhuh009_lab10_part1.c
 *
 * Created: 2/12/2019 2:45:08 PM
 * Author : Patrick Wumbo
 */ 

#include <avr/io.h>
#include "timer.h"

enum States { Start, S0, S1, S2} state;
unsigned char tmpB = 0x00;
unsigned char tmpC = 0x01;

void Tick() {
	switch(state) {
		case Start: state = S0; break;
		case S0: 
			state = S1; break;
		case S1: 
			state = S2; break;
		case S2: 
			state = S0; break;
		default: 
			state = Start; break;
	}
	switch(state) {
		case Start: 
			break;
				
		case S0:
			tmpB = 0x01; 
			if(tmpC == 0x01) {
				tmpB += 0x08;
				tmpC = 0x00;
			}
			else {
				tmpC = 0x01;
			}
			PORTB = tmpB; break;
		case S1: 
			tmpB = 0x02; 
			if(tmpC == 0x01) {
				tmpB += 0x08;
				tmpC = 0x00;
			}
			else {
				tmpC = 0x01;
			}
			PORTB = tmpB; break;
		case S2: 
			tmpB = 0x04; 
			if(tmpC == 0x01) {
				tmpB += 0x08;
				tmpC = 0x00;
			}
			else {
				tmpC = 0x01;
				
			}
			PORTB = tmpB; break;
		default: break;
	}
}



int main(void)
{
	DDRB = 0xFF;
	PORTB = 0x00;
	TimerSet(1000);
	TimerOn();
	while (1)
	{
		Tick();
		while(!TimerFlag);
		TimerFlag = 0;
	}
}

