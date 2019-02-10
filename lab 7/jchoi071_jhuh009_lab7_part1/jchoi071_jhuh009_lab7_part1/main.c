#include <avr/io.h>
#include "io.c"
#include "timer.h"
enum IncDec_States { Start, Init, WaitRise, WaitCnt, Inc, Dec, Reset } IncDec_State;

//unsigned char tmpA = 0x00;
unsigned char tmpC = 0x00;
unsigned char buttonA = 0x00;
unsigned char buttonB = 0x00;
unsigned char buttonC = 0x00;

void TickFct_IncDec()
{
	switch (IncDec_State)
	{
		case Start:
			IncDec_State = Init;
			break;
		
		case Init:
			IncDec_State = WaitRise;
			break;
		
		case WaitRise: 	
			buttonA = ~PINA & 0x01;
			buttonB = ~PINA & 0x02;
			buttonC = ~PINA & 0x04;
			if(buttonA) {
				IncDec_State = Inc;
				break;
			}
			else if(buttonB) {
				IncDec_State = Dec;
				break;
			}
			else if(buttonC) {
				IncDec_State = Reset;
			}
			else {
				IncDec_State = WaitRise;
				break;
			}
			break;
			
		case WaitCnt:
			IncDec_State = WaitRise;
			break;
			
		case Inc:
			IncDec_State = WaitRise;
			break;
			
		case Dec:
			IncDec_State = WaitRise;
			break;
						
		case Reset:
			IncDec_State = WaitRise;
			break;
		default:
			IncDec_State = WaitRise;
			break;
		
	}
	
	switch (IncDec_State)
	{
		case Start:

			break;
		
		case Init:
			tmpC = 0;
			LCD_init();
			
			break;
		
		case WaitRise:
			LCD_ClearScreen();
			LCD_Cursor(1);
			LCD_WriteData(tmpC + '0');
			break;

// 		case WaitFall:
// 			break;
		
		case Inc:
			if ((tmpC + 1) <= 9)
			{
				++tmpC;
			}
			//PORTC = tmpC;
			break;
		
		case Dec:
			if ((tmpC - 1) >= 0)
			{
				--tmpC;
			}
			//PORTC = tmpC;
			break;
		
		case Reset:
			tmpC = 0;
			//PORTC = tmpC;
			break;
		
		default:
			break;
	}
	LCD_ClearScreen();
	LCD_Cursor(1);
	LCD_WriteData(tmpC + '0');		
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs
	IncDec_State = Start;
	TimerSet(200);
 	TimerOn();
	while (1)
	{
 		TickFct_IncDec();
		 while(!TimerFlag);
		 TimerFlag = 0;
	}
}

