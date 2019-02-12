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

enum States { Start, S0, S1, S2, S3, S4 } state;
unsigned char tmpA = 0x00;
unsigned char tmpB = 0x00;

void Tick()
{
	tmpA = ~PINA;
	
	switch(state)
	{
		case Start: state = S0; break;
		case S0: state = S1; break;
		case S1: 
			if (tmpA & 0x01)
			{
				state = S3;
			}
			else state = S2; break;
		case S2: state = S0; break;
		case S3:
			if (tmpA & 0x01)
			{
				state = S3;
			}
			else state = S4; break;
			
		case S4:
			if (tmpA & 0x01)
			{
				state = S1;
			}
			else state = S4; break;
		default: state = Start; break;
	}
	switch(state)
	{
		case Start: break;
		case S0: tmpB = 0x01; break;
		case S1: tmpB = 0x02; break;
		case S2: tmpB = 0x04; break;
		case S3: tmpB = 0x02; break;
		case S4: tmpB = 0x02; break;
		default: break;
	}
	
	PORTB = tmpB;
}


int main(void)
{
	DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0xFF;
	PORTB = 0x00;
	TimerSet(300);
	TimerOn();
    while (1) 
    {
		Tick();
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

