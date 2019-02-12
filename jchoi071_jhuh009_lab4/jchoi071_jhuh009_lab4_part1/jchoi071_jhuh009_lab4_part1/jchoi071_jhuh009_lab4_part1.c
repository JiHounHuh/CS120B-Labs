/*
 * jchoi071_jhuh009_lab4_part1.c
 *
 * Created: 1/17/2019 2:53:34 PM
 * Author : Patrick Wumbo
 */ 

#include <avr/io.h>
//#include "RIMS.h"
enum States{Start, Init, PBOne} state;
unsigned char tmpA = 0x00;
unsigned char tmpB = 0x00;
void tick() {
	switch(state) {
		case Start:
			state = Init;
			break;
		
		case Init:
			tmpA = PINA;
			tmpB = PINB;
			if(!(tmpA & 0x01)) {
				state = Init;
			}
			else if(tmpA & 0x01) {
				 state = PBOne;
			}
			break;
		
		case PBOne:
			if(!(tmpA & 0x01)) {
				state = PBOne;
			}
			else if(tmpA & 0x01) {
				 state = Init;
			}
			break;
		default:
			state = Start;
			break;
	}
	
	switch(state) {
		case Init:
			
			PORTB = 0x01;
			break;
		
		case PBOne:
			PORTB = 0x02;
			break;
			
		default:
			break;	
	}	
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	state = Start;
	
	while(1) {
		tick();
	}
}

