/*  Partner(s) Name & E-mail: Ji Hoon Choi (jchoi071@ucr.edu), Ji Houn Huh (jhuh009@ucr.edu)
 *  Lab Section: 23
 *  Assignment: Lab # 7 Exercise # 2
 *  Exercise Description: Light game with LCD
 *  
 *  I acknowledge all content contained herein, excluding template or example
 *  code, is my own original work.
 */

#include <avr/io.h>
#include "io.c"
#include "timer.h"
enum IncDec_States { Start, Init, WaitRise, WaitCnt, Inc, Dec, Win } IncDec_State;

//unsigned char tmpA = 0x00;
unsigned char tmpB = 0x00;
unsigned char tmpC = 0x00;
unsigned char buttonA = 0x00;
const unsigned char WINMSG[] = "You win!";

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

		    if(buttonA && ((tmpB & 0x02) == 0x02))
            {
    		    IncDec_State = Inc;
    		    break;
		    }

		    else if(buttonA && ((tmpB & 0x02) != 0x02))
            {
    		    IncDec_State = Dec;
    		    break;
		    }
            
            else if (tmpC >= 9)
            {
                IncDec_State = Win;
            }

		    else
            {
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
		
        case Win:
            IncDec_State = Win;
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
            tmpB = 0x01;
		    LCD_init();
		    break;
		
		case WaitRise:
		    LCD_ClearScreen();
		    LCD_Cursor(1);
		    LCD_WriteData(tmpC + '0');
        
            if (tmpB < 4)
            {
                tmpB = tmpB << 1;
            }
            else if (tmpB >= 4)
            {
                tmpB = 0x01;
            }
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
			    --tmpC;
		    }
		    break;
		
        case Win:
            if (tmpC >= 9)
            {
                LCD_DisplayString(1, WINMSG);
                tmpB = 0;
                tmpC = 0;
            }            
            break;

		default:
		    break;
	}
	//LCD_ClearScreen();
	//LCD_Cursor(1);
	//LCD_WriteData(tmpC + '0');
    PORTB = tmpB;
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
    DDRB = 0xFF; PORTB = 0x00;
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

