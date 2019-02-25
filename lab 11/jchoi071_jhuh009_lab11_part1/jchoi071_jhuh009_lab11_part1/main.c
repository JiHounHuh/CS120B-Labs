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

unsigned char GetKeypadKey() {

	PORTC = 0xEF; // Enable col 4 with 0, disable others with 1’s
	asm("nop"); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC,0)==0) { return('1'); }
	if (GetBit(PINC,1)==0) { return('4'); }
	if (GetBit(PINC,2)==0) { return('7'); }
	if (GetBit(PINC,3)==0) { return('*'); }

	// Check keys in col 2
	PORTC = 0xDF; // Enable col 5 with 0, disable others with 1’s
	asm("nop"); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC,0)==0) { return('2'); }
	if (GetBit(PINC,1)==0) { return('5'); }
	if (GetBit(PINC,2)==0) { return('8'); }
	if (GetBit(PINC,3)==0) { return('0'); }

	// Check keys in col 3
	PORTC = 0xBF; // Enable col 6 with 0, disable others with 1’s
	asm("nop"); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC,0)==0) { return('3'); }
	if (GetBit(PINC,1)==0) { return('6'); }
	if (GetBit(PINC,2)==0) { return('9'); }
	if (GetBit(PINC,3)==0) { return('#'); }

	// Check keys in col 4
	PORTC = 0x7F; // Enable col 7 with 0, disable others with 1’s
	asm("nop"); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC,0)==0) { return('A'); }
	if (GetBit(PINC,1)==0) { return('B'); }
	if (GetBit(PINC,2)==0) { return('C'); }
	if (GetBit(PINC,3)==0) { return('D'); }

	return('\0'); // default value

}
//--------User defined FSMs---------------------------------------------------
//Enumeration of states.
enum SM1_States { SM1_Start, SM1_wait, SM1_keypadPress};

// Monitors button connected to PA0. 
// When button is pressed, shared variable "pause" is toggled.
int SMTick1(int state) {

	// Local Variables
	unsigned char keypadPress = GetKeypadKey();
	
	//State machine transitions
	switch (state) {
	case SM1_Start:
		state = SM1_wait;
		break;
		
	case SM1_wait:
		if(keypadPress != '\0'){
			state = SM1_keypadPress;
		} 	
		break;

	case SM1_keypadPress:	
		if(keypadPress == '\0'){
			state = SM1_wait;
		}else{
			state = SM1_keypadPress;
		}
		break;
		
	default:		
		state = SM1_wait; // default: Initial state
		break;
	}

	//State machine actions
	switch(state) {
		case SM1_wait:	
			keypadPress = 0x1F;

		case SM1_keypadPress:	
			switch (keypadPress) {
				case '\0': PORTB = 0x1F; break; // All 5 LEDs on
				case '0': PORTB = 0x00; break;
				case '1': PORTB = 0x01; break; // hex equivalent
				case '2': PORTB = 0x02; break;
				case '3': PORTB = 0x03; break;
				case '4': PORTB = 0x04; break;
				case '5': PORTB = 0x05; break;
				case '6': PORTB = 0x06; break;
				case '7': PORTB = 0x07; break;
				case '8': PORTB = 0x08; break;
				case '9': PORTB = 0x09; break;
				case 'A': PORTB = 0x0A; break;
				case 'B': PORTB = 0x0B; break;
				case 'C': PORTB = 0x0C; break;
				case 'D': PORTB = 0x0D; break;
				case '*': PORTB = 0x0E; break;
				case '#': PORTB = 0x0F; break;
				default: PORTB = 0x1F; break; // Should never occur. Middle LED off.
			}
			break;

		default:		
	break;
	}

	return state;
}

// Implement scheduler code from PES.
int main()
{
// Set Data Direction Registers
// Buttons PORTA[0-7], set AVR PORTA to pull down logic
DDRC = 0xF0; PORTA = 0x0F;
DDRB = 0xFF; PORTB = 0x00;

//Declare an array of tasks 
static task task1;
task *tasks = &task1;


// Task 1
task1.state = -1;//Task initial state.
task1.period = 50;//Task Period.
task1.TickFct = &SMTick1;//Function pointer for the tick.

// Set the timer and turn it on
TimerSet(50);
TimerOn();

unsigned short i; // Scheduler for-loop iterator
while(1) {
	// Scheduler code
	for ( i = 0; i < 1; i++ ) {
			tasks->state = tasks->TickFct(tasks->state);
	}
	while(!TimerFlag);
	TimerFlag = 0;
}

// Error: Program should not exit!
}
