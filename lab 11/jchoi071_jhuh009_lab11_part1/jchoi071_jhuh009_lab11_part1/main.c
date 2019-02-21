/*
 * jchoi071_jhuh009_lab11_part1.c
 *
 * Created: 2/19/2019 2:40:34 PM
 * Author : Patrick Wumbo
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "bit.h"
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

//--------End Task scheduler data structure-----------------------------------

//--------Shared Variables----------------------------------------------------
unsigned char SM2_output = 0x00;
unsigned char SM3_output = 0x00;
unsigned char pause = 0;

//--------End Shared Variables------------------------------------------------

//--------User defined FSMs---------------------------------------------------
//Enumeration of states.
enum SM1_States { SM1_wait, SM1_press, SM1_release };

// Monitors button connected to PA0. 
// When button is pressed, shared variable "pause" is toggled.
int SMTick1(int state) {

	// Local Variables
	unsigned char press = ~PINC;

	//State machine transitions
	switch (state) {
	case SM1_wait: 	
		if (press == 0x01) {	// Input 1
			state = SM1_press;
		}
		if (press == 0x02) {	// Input 2
			state = SM1_press;
		}
// 		if (press == 0x03) {	// Input 3
// 			state = SM1_press;
// 		}
// 		if (press == 0x04) {	// Input 4
// 			state = SM1_press;
// 		}
// 		if (press == 0x05) {	// Input 5
// 			state = SM1_press;
// 		}
// 		if (press == 0x06) {	// Input 6
// 			state = SM1_press;
// 		}
// 		if (press == 0x07) {	// Input 7
// 			state = SM1_press;
// 		}
// 		if (press == 0x08) {	// Input 8
// 			state = SM1_press;
// 		}
// 		if (press == 0x09) {	// Input 9
// 			state = SM1_press;
// 		}
// 		if (press == 0x0A) {	// Input A
// 			state = SM1_press;
// 		}
// 		if (press == 0x0B) {	// Input B
// 			state = SM1_press;
// 		}
// 		if (press == 0x0C) {	// Input C
// 			state = SM1_press;
// 		}
// 		if (press == 0x0D) {	// Input D
// 			state = SM1_press;
// 		}
// 		if (press == 0x0E) {	// Input #
// 			state = SM1_press;
// 		}
// 		if (press == 0x0F) {	// Input 0
// 			state = SM1_press;
// 		}
// 		if (press == 0x10) {	// Input *
// 			state = SM1_press;
// 		}
		break;

	case SM1_press:	
		state = SM1_release;
		break;

	case SM1_release:	
		if (press == 0x00) {	// Wait for button release
	state = SM1_wait;
	}
break;

	default:		state = SM1_wait; // default: Initial state
				break;
	}

	//State machine actions
	switch(state) {
	case SM1_wait:	
			PORTB = press;
		
		break;

	case SM1_press:	break;

	case SM1_release:	break;

	default:		break;
	}

	return state;
}

// Implement scheduler code from PES.
int main()
{
// Set Data Direction Registers
// Buttons PORTA[0-7], set AVR PORTA to pull down logic
DDRC = 0x00; PORTA = 0xFF;
DDRB = 0xFF; PORTB = 0x00;
// . . . etc

// Period for the tasks
unsigned long int SMTick1_calc = 50;
unsigned long int SMTick2_calc = 500;
unsigned long int SMTick3_calc = 1000;
unsigned long int SMTick4_calc = 10;

//Calculating GCD
unsigned long int tmpGCD = 1;
tmpGCD = findGCD(SMTick1_calc, SMTick2_calc);
tmpGCD = findGCD(tmpGCD, SMTick3_calc);
tmpGCD = findGCD(tmpGCD, SMTick4_calc);

//Greatest common divisor for all tasks or smallest time unit for tasks.
unsigned long int GCD = tmpGCD;

//Recalculate GCD periods for scheduler
unsigned long int SMTick1_period = SMTick1_calc/GCD;
unsigned long int SMTick2_period = SMTick2_calc/GCD;
unsigned long int SMTick3_period = SMTick3_calc/GCD;
unsigned long int SMTick4_period = SMTick4_calc/GCD;

//Declare an array of tasks 
static task task1, task2, task3, task4;
task *tasks[] = { &task1, &task2, &task3, &task4 };
const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

// Task 1
task1.state = -1;//Task initial state.
task1.period = SMTick1_period;//Task Period.
task1.elapsedTime = SMTick1_period;//Task current elapsed time.
task1.TickFct = &SMTick1;//Function pointer for the tick.

// Set the timer and turn it on
TimerSet(GCD);
TimerOn();

unsigned short i; // Scheduler for-loop iterator
while(1) {
	// Scheduler code
	for ( i = 0; i < numTasks; i++ ) {
		// Task is ready to tick
		if ( tasks[i]->elapsedTime == tasks[i]->period ) {
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
