/*  Partner(s) Name & E-mail: Ji Hoon Choi (jchoi071@ucr.edu), Ji Houn Huh (jhuh009@ucr.edu)
 *  Lab Section: 23
 *  Assignment: Lab # 11 Exercise # 5
 *  Exercise Description: Simple Game
 *  
 *  I acknowledge all content contained herein, excluding template or example
 *  code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "bit.h"
#include "io.c"
#include "timer.h"
#include "stdio.h"

//--------Find GCD function --------------------------------------------------
unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
    unsigned long int c;
    while(1){
        c = a%b;
        if(c==0){return b;}
        a = b;
        b = c;
    }
    return 0;
}
//--------End find GCD function ----------------------------------------------

//--------Task scheduler data structure---------------------------------------
// Struct for Tasks represent a running process in our simple real-time operating system.
typedef struct _task {
    /*Tasks should have members that include: state, period,
        a measurement of elapsed time, and a function pointer.*/
    signed char state; //Task's current state
    unsigned long int period; //Task period
    unsigned long int elapsedTime; //Time elapsed since last task tick
    int (*TickFct)(int); //Task tick function
} task;

//--------Shared Variables----------------------------------------------------
char keypadChar = 0;
unsigned char counter = 0;
char column = 1;
char row1 = 16;
char row2 = 32;
char input = 0;
const char MSG[] = "Bad Move";
const char OBS[] = "#";
//const char MSG[] = "Woomy!"; //Test message, shorter than buffer
char buffer[] = "               ";
unsigned char pos = 0;
const unsigned char BUFFER_SIZE = 16;
unsigned char msg_size = strlen(MSG);
//--------End Shared Variables------------------------------------------------

//--------End Task scheduler data structure-----------------------------------



//--------User defined FSMs---------------------------------------------------
unsigned char GetButtonValue(){
	PORTC = 0xFF;
	asm("nop");
	if (GetBit(PINC,0)==0) { return('1'); }
	if (GetBit(PINC,1)==0) { return('2'); }
	if (GetBit(PINC,2)==0) { return('3'); }
	return ('0');
}
//Enumeration of states.
enum SM1_States { SM1_Start, SM1_wait, SM1_ButtonPress, SM1_Pause, SM1_Up, SM1_Down };

int SMTick1(int state)
{
	// Local Variables
	unsigned char button = GetButtonValue();
	
	
	//State machine transitions
	switch (state) {
		case SM1_Start:
		state = SM1_wait;
		break;
		
		case SM1_wait:
			if(button != '0') {
				state = SM1_ButtonPress;
				break;
			}
		break;

		case SM1_Pause:
			state = SM1_ButtonPress;
			break;
			
		case SM1_Up:
			state = SM1_ButtonPress;
			break;
			
		case SM1_Down:
			state = SM1_ButtonPress;
			break;
		
		
		case SM1_ButtonPress:
			if(button == '0'){
				state = SM1_wait;
			}else{
				state = SM1_ButtonPress;
			}
			break;
		
		default:
		state = SM1_wait; // default: Initial state
		break;
	}

	//State machine actions
	switch(state) {
		case SM1_wait:
		break;

		case SM1_ButtonPress:
		switch (button) {
			asm("nop");
			case '0': break;
			case '1': input = '1'; break;
			case '2': input = '2'; break;
			case '3': input = '3'; break;
			default: input = 0x00; break;
		}
		break;

		default:
		break;
	}

	return state;
}

// Enumeration of states.
enum SM2_States { SM2_Start, SM2_Display };

int SMTick2(int state)
{
	switch (state)
	{
		case SM2_Start:
		state = SM2_Display;
		break;
		
		case SM2_Display:
		//state = SM2_Display;
		break;
		
		default:
		state = SM2_Display;
		break;
	}
	
	switch (state)
	{
		case SM2_Start:
		break;
		
		case SM2_Display:
		if(input == 1){
			LCD_Cursor(17);
			//if(input == column) LCD_DisplayString(2,MSG);
			break;
		}
		if(input == 2){
			LCD_Cursor(1);
			//if(input == column) LCD_DisplayString(2,MSG);
			break;
		}
		break;
		
		default:
		break;
	}
	

	return state;
}

enum DisplayStates { Start1, Display1 } DisplayState;

int SMTick3()
{
    unsigned char counter = 0;
    switch (DisplayState)
    {
        case Start1:
            DisplayState = Display1;
            break;
            
        case Display1:
            DisplayState = Display1;
            break;
            
        default:
            DisplayState = Display1;
            break;
    }
    
    switch (DisplayState)
    {
        case Start1:
            break;
            
        case Display1:
            if (!strcmp(buffer, "                ") && pos != 0)
            {
                pos = 0;
            }
            if (pos == 0)
            {
                buffer[BUFFER_SIZE - 1] = OBS[0];
            }
            else if ((pos > 0) && (pos < msg_size))
            {
                for (counter = 1; counter < BUFFER_SIZE; ++counter)
                {
                    buffer[counter - 1] = buffer[counter];
                }
                buffer[BUFFER_SIZE - 1] = OBS[pos];
            }
            else
            {
                for (counter = 1; counter < BUFFER_SIZE; ++counter)
                {
                    buffer[counter - 1] = buffer[counter];
                }
                buffer[BUFFER_SIZE - 1] = 0x20;
            }
            ++pos;
            LCD_DisplayString(1, buffer);
            break;
    }
}

// enum DisplayStates2 { Start2, Display2 } DisplayState2;
// 
// void TickFct_DisplayMsg()
// {
// 	unsigned char counter = 0;
// 	switch (DisplayState2)
// 	{
// 		case Start2:
// 		DisplayState2 = Display2;
// 		break;
// 		
// 		case Display2:
// 		DisplayState2 = Display2;
// 		break;
// 		
// 		default:
// 		DisplayState2 = Display2;
// 		break;
// 	}
// 	
// 	switch (DisplayState2)
// 	{
// 		case Start2:
// 		break;
// 		
// 		case Display2:
// 		if (!strcmp(buffer, "                ") && pos != 0)
// 		{
// 			pos = 0;
// 		}
// 		if (pos == 0)
// 		{
// 			buffer[BUFFER_SIZE - 1] = MSG[0];
// 		}
// 		else if ((pos > 0) && (pos < msg_size))
// 		{
// 			for (counter = 1; counter < BUFFER_SIZE; ++counter)
// 			{
// 				buffer[counter - 1] = buffer[counter];
// 			}
// 			buffer[BUFFER_SIZE - 1] = MSG[pos];
// 		}
// 		else
// 		{
// 			for (counter = 1; counter < BUFFER_SIZE; ++counter)
// 			{
// 				buffer[counter - 1] = buffer[counter];
// 			}
// 			buffer[BUFFER_SIZE - 1] = 0x20;
// 		}
// 		++pos;
// 		LCD_DisplayString(17, buffer);
// 		break;
// 	}
// }

int main()
{
	// Set Data Direction Registers
	DDRA = 0xFF; PORTA = 0x00; // LCD control lines
	DDRC = 0xF0; PORTC = 0x0F; // Keypad
	DDRD = 0xFF; PORTD = 0x00; // LCD data lines

	// Period for the tasks
	unsigned long int SMTick1_calc = 50;
	unsigned long int SMTick2_calc = 500;
	unsigned long int SMTick3_calc = 100;

	// Calculating GCD
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD (SMTick1_calc, SMTick2_calc);
	tmpGCD = findGCD(tmpGCD, SMTick3_calc);

	// Greatest common divisor for all tasks or smallest time unit for tasks
	unsigned long int GCD = tmpGCD;

	//Recalculate GCD periods for scheduler
	unsigned long int SMTick1_period = SMTick1_calc / GCD;
	unsigned long int SMTick2_period = SMTick2_calc / GCD;
	unsigned long int SMTick3_period = SMTick3_calc / GCD;

	// Declare an array of tasks
	static task task1, task2, task3;
	task *tasks[] = { &task1, &task2, &task3 };
	const unsigned short numTasks = sizeof(tasks) / sizeof(task*);

	// Task 1
	task1.state = -1; // Task initial state.
	task1.period = SMTick1_period; // Task Period.
	task1.elapsedTime = SMTick1_period; // Task current elapsed time.
	task1.TickFct = &SMTick1; // Function pointer for the tick.

	//Task 2
	LCD_init();
	task2.state = -1; // Task initial state.
	task2.period = SMTick2_period; // Task Period.
	task2.elapsedTime = SMTick2_period; // Task current elapsed time.
	task2.TickFct = &SMTick2; // Function pointer for the tick.
	
	//Task 3
	LCD_init();
	task3.state = -1; // Task initial state.
	task3.period = SMTick2_period; // Task Period.
	task3.elapsedTime = SMTick2_period; // Task current elapsed time.
	task3.TickFct = &SMTick3; // Function pointer for the tick.

	// Set the timer and turn it on
	TimerSet(GCD);
	TimerOn();

	unsigned short i; // Scheduler for-loop iterator
	while(1)
	{
		// Scheduler code
		for ( i = 0; i < numTasks; i++ )
		{
			// Task is ready to tick
			if ( tasks[i]->elapsedTime == tasks[i]->period )
			{
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset the elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}

	// Error: Program should not exit!
	return 0;
}


