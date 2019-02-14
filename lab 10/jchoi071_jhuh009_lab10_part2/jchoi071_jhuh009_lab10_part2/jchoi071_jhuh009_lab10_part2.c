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
			state = S0; 
			break;
		default: 
			state = Start; break;
	}
	switch(state) {
		case Start: 
			break;
				
		case S0:
			tmpB = 0x01; 
			PORTB = tmpB; break;
		case S1: 
			tmpB = 0x02; 
			PORTB = tmpB; break;
		case S2: 
			tmpB = 0x04; 
			PORTB = tmpB; break;
		default: break;
	}
}
enum BlinkStates {Begin, S3} BlinkState;
void TickFct_BlinkLed() {
	switch(BlinkState) {
		case Begin:
			BlinkState = S3;
			break;
		
		case S3: 
			break;
		default:
			BlinkState = Begin;
			break;
	}
	switch(BlinkState){
		case Begin:
			break;
		case S3:
			if(tmpC == 0x01) {
				tmpB += 0x08;
				tmpC = 0x00;
			}
			else {
				tmpC = 0x01;
			}
			PORTB = tmpB;
			break;
		default:
			break;
	}
	
}

int main(void)
{
	unsigned long BL_elapsedTime = 0;
	unsigned long TL_elapsedTime = 0;
	const unsigned long timerPeriod = 100;
	DDRB = 0xFF;
	PORTB = 0x00;
	TimerSet(100);
	TimerOn();
	while (1)
	{
		if (BL_elapsedTime >= 1000) { // 1000 ms period
			TickFct_BlinkLed(); // Execute one tick of the BlinkLed synchSM
			BL_elapsedTime = 0;
		}
		if(TL_elapsedTime >= 300){
			Tick();
			TL_elapsedTime = 0;
		}
		
		//TickFct_BlinkLed();
		while(!TimerFlag);
		TimerFlag = 0;
		BL_elapsedTime += timerPeriod;
		TL_elapsedTime += timerPeriod;
	}
}

