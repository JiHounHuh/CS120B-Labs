/*
 * jchoi071_jhuh009_lab5_part2.c
 *
 * Created: 1/24/2019 9:35:14 AM
 * Author : Patrick Wumbo
 */ 

#include <avr/io.h>

enum IncDec_States { Start, Init, WaitRise, WaitFall, Inc, Dec, Reset } IncDec_State;

unsigned char tmpA = 0x00;
unsigned char tmpB = 0x00;
unsigned char button = 0x00;

void TickFct_IncDec()
{
	switch (IncDec_State)
	{
		case Start:
		IncDec_State = Init;
		break;
		
		case Init:
		tmpA = PINA;
		button = ~tmpA & 0x0F;
		IncDec_State = WaitRise;
		break;
		
		case WaitRise:
		
		if (GetBit) && (!(tmpA & 0x02)))
		{
			IncDec_State = Inc;
		}
		else if ((!(tmpA & 0x01)) && (tmpA & 0x02))
		{
			IncDec_State = Dec;
		}
		else if ((tmpA & 0x03) == 0x03)
		{
			IncDec_State = Reset;
		}
		else if ((tmpA & 0x03) != 0x03)
		{
			IncDec_State = WaitRise;
		}
		else
		{
			IncDec_State = WaitRise;
		}
		break;
		
		case WaitFall:
		if (tmpA == PINA)
		{
			IncDec_State = WaitFall;
		}
		else if (tmpA != PINA)
		{
			IncDec_State = WaitRise;
		}
		else
		{
			IncDec_State = WaitRise;
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
		IncDec_State = WaitFall;
		break;
		
	}
	
	switch (IncDec_State)
	{
		case Start:
		break;
		
		case Init:
		tmpC = 7;
		break;
		
		case WaitRise:
		PORTC = tmpC;
		break;

		case WaitFall:
		PORTC = tmpC;
		break;
		
		case Inc:
		if (tmpC + 1 <= 9)
		{
			++tmpC;
		}
		break;
		
		case Dec:
		if (tmpC - 1 >= 0)
		{
			--tmpC;
		}
		break;
		
		case Reset:
		tmpC = 0;
		
		default:
		break;
	}
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs
	
	IncDec_State = Start;
	while (1)
	{
 		TickFct_IncDec();
	}
}

