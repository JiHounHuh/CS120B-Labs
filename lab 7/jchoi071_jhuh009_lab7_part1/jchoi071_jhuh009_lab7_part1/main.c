#include <avr/io.h>
#include "io.c"
enum IncDec_States { Start, Init, WaitRise, WaitFall, Inc, Dec, Reset } IncDec_State;

//unsigned char tmpA = 0x00;
unsigned char tmpC = 0x00;
unsigned char buttonA = 0x00;
unsigned char buttonB = 0x00;

void TickFct_IncDec()
{
	switch (IncDec_State)
	{
		case Start:
		IncDec_State = Init;
		break;
		
		case Init:
		//tmpA = PINA;
		
		IncDec_State = WaitRise;
		break;
		
		case WaitRise:
		
		buttonA = ~PINA & 0x01;
		buttonB = ~PINA & 0x02;
		if(buttonA)
		//if ((buttonA) && (!(buttonB)))
		{
			IncDec_State = Dec;
			break;
		}
		else if(buttonB)
		//else if ((!(buttonA)) && (buttonB))
		{
			IncDec_State = Reset;
			break;
		}
		else
		{
			IncDec_State = WaitRise;
			break;
		}
		break;
		
		case WaitFall:
			buttonA = ~PINA & 0x01;
			buttonB = ~PINA & 0x02;
			if (buttonA)
			{
				IncDec_State = WaitFall;
				break;
			}
			else if (buttonB)
			{
				IncDec_State = WaitRise;
				break;
			}
			else if (!buttonB)
			{
				IncDec_State = WaitRise;
				break;
			}
			else
			{
				IncDec_State = WaitRise;
				break;
			}
			break;

		case Inc:
		IncDec_State = WaitFall;
		break;
		
		case Dec:
		IncDec_State = WaitFall;
		break;
		
		case Reset:
		IncDec_State = WaitFall;
		break;
		
		default:
		IncDec_State = WaitRise;
		break;
		
	}
	
	switch (IncDec_State)
	{
		case Start:
// 			LCD_ClearScreen();
// 			LCD_Cursor(1);
			break;
		
		case Init:
			tmpC = 0;
			LCD_Cursor(0);
			//LCD_init();
			break;
		
		case WaitRise:
			
			//LCD_DisplayString(0, tmpC + '0');
			break;

		case WaitFall:
		//LCD_WriteData( tmpC + '0' ); 
		break;
		
		case Inc:
		if ((tmpC + 1) <= 9)
		{
			++tmpC;
		}
		break;
		
		case Dec:
		if ((tmpC - 1) >= 0)
		{
			tmpC = tmpC - 1;
		}
		break;
		
		case Reset:
		tmpC = 0;
		break;
		
		default:
		break;
	}
	LCD_Cursor(0);
	LCD_WriteData(tmpC + '0' );
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs
	//unsigned char led = 0x00;
	LCD_init();
	LCD_ClearScreen();
	//LCD_DisplayString(1, "I start");
	IncDec_State = Start;
	while (1)
	{
 		TickFct_IncDec();
		// LCD_DisplayString(1, tmpC);
	}
}

