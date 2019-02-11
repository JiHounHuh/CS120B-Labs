/*
 * jhuh009_jchoi071_lab4_part3.c
 *
 * Created: 1/19/2019 7:24:05 PM
 * Author : Patrick Wumbo
 */ 

#include <avr/io.h>

enum DoorLock_States {Start, Init, KeyY, KeyP, Wait, DoorUnlock, DoorLock } DoorLock_State;
unsigned char tmpA = 0x00;
//unsigned char tmpC = 0x00;

void Tick_DoorLock() {
	switch (DoorLock_State)
	{
		case Start:
			DoorLock_State = Init;
			break;
		
		case Init:
			tmpA = PINA;
			if((tmpA & 0x04)) {
				DoorLock_State = KeyP;
			}
			else if((tmpA & 0x80)) {
				DoorLock_State = DoorLock;
			}
			else {
				DoorLock_State = Init;
			}
			break;
		
		case KeyY:
			DoorLock_State = DoorUnlock;
			break;
		
		case KeyP:
			DoorLock_State = Wait;
			break;
			
		case Wait:
			if((tmpA & 0x02)) {
				DoorLock_State = KeyY;
			}
			else if((tmpA & 0x80)) {
				DoorLock_State = DoorLock;
			}
			else if(tmpA & 0x01) {
				DoorLock_State = Init;
			}
			else {
				DoorLock_State = Wait;
			}
			break;
			
		case DoorLock:
			if((tmpA & 0x00)) {
				DoorLock_State = Init;
			}
			else {
				DoorLock_State = DoorLock;
			}
			break;
		
		case DoorUnlock:
			if((tmpA & 0x87)) {
				DoorLock_State = Init;
			}
			else {
				DoorLock_State = DoorUnlock;
			}
			break;
		
		default:
			break;
	}
	switch (DoorLock_State) 
	{
		case Start:
			PORTB = 0x00;
			PORTC = 0x00;
			break;
		
		case Init:
			break;
			
		case KeyP:
			PORTC = 0x04;
			break;
		
		case KeyY:
			PORTB = 0x01;
			PORTC = 0x02;
			break;
		
		case Wait:
			PORTC = 0x03;
			break;
			
		case DoorUnlock:
			PORTC = 0x05;
			PORTB = 0x01;
			break;
			
		case DoorLock:
			PORTB = 0x00;
			PORTC = 0x06;
			break;
			
	}
	
}
int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DoorLock_State = Start;
	while (1)
	{
		Tick_DoorLock();
	}
}
