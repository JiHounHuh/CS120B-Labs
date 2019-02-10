 /*
 * jchoi071_jhuh009_lab9_part1.c
 *
 * Created: 2/7/2019 2:40:01 PM
 * Author : Patrick Wumbo
 */ 

#include <avr/io.h>


// 0.954 hz is lowest frequency possible with this function,
// based on settings in PWM_on()
// Passing in 0 as the frequency will stop the speaker from generating sound
void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR0B &= 0x08; } //stops timer/counter
		else { TCCR0B |= 0x03; } // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR0A = 0xFFFF; }
		
		// prevents OCR0A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR0A = 0x0000; }
		
		// set OCR3A based on desired frequency
		else { OCR0A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT0 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}

void PWM_on() {
	TCCR0A = (1 << COM0A0) | (1 << WGM00);
	// COM3A0: Toggle PB3 on compare match between counter and OCR0A
	TCCR0B = (1 << WGM02) | (1 << CS01) | (1 << CS00);
	// WGM02: When counter (TCNT0) matches OCR0A, reset counter
	// CS01 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR0A = 0x00;
	TCCR0B = 0x00;
}
enum Sound_States {Start, Init, WaitRise, WaitFall, SetA, Inc, Dec, Speaker} Sound_State;
unsigned char tmpA = 0x01;
unsigned char tmpC = 0x00;
unsigned char buttonA = 0x00;
unsigned char buttonB = 0x00;
unsigned char buttonC = 0x00;

void TickFct_Sound() {
	switch(Sound_State) {
		case Start:
			Sound_State = Init;
			break;
		
		case Init:
			Sound_State = WaitRise;
			
			break;
		
		case WaitRise:
			buttonA = ~PINA & 0x01;
			buttonB = ~PINA & 0x02;
			buttonC = ~PINA & 0x04;
			if(buttonA && !buttonB && !buttonC) {
				Sound_State = SetA;
				break;	
			}
			else if(tmpA == 0x01) {
				if(buttonB && !buttonA && !buttonC) {
					Sound_State = Inc;
					break;
				}
				else if(buttonC && !buttonA && !buttonB) {
					Sound_State = Dec;
					break;
				}
			}
			else {
				Sound_State = WaitRise;
				break;
			}
			break;
			
		case WaitFall:
			buttonA = ~PINA & 0x01;
			buttonB = ~PINA & 0x02;
			buttonC = ~PINA & 0x04;
			if(buttonA && !buttonB && !buttonC) {
    			Sound_State = WaitFall;
    			break;
			}
			else if(buttonB && !buttonA && !buttonC) {
				Sound_State = WaitFall;
				break;
			}
			else if(buttonC && !buttonA && !buttonB) {
				Sound_State = WaitFall;
				break;
			}
			else {
				Sound_State = Speaker;
				break;
			}
			break;
			
		case Speaker:
			Sound_State = WaitRise;
			break;
			
		case SetA:
			Sound_State = WaitFall;
			break;
		
		case Inc:
			Sound_State = WaitFall;
			break;
		
		case Dec:
			Sound_State = WaitFall;
			break;
			
		default:
			Sound_State = WaitRise;
			break;
	}
	
	switch(Sound_State) {
		case Start:
			break;
		//enum Sound_States {Start, Init, WaitRise, WaitFall, SetA, SetB, SetC} Sound_State;
		case Init:
			tmpC = 0;
			tmpA = 0;
			set_PWM(0);
			break;
			
		case WaitRise:
			//set_PWM(0);
			//(PORTB & 0x04) = tmpC;
			break;
			
		case WaitFall:
			break;
			
		case SetA:
			if(tmpA == 0x00) {
				tmpA = 1;
				set_PWM(1);
			}
			else if(tmpA == 0x01) {
				tmpA = 0;
				set_PWM(0);
			}
			break;
			
		case Inc:
			if ((tmpC + 1) < 9)
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
		
		case Speaker:
			if(tmpA == 0) {
				break;
			}
			else {
				if(tmpC == 0) {
					set_PWM(261.63);
					break;
				}
				else if(tmpC == 1) {
					set_PWM(293.66);
					break;
				}
				else if(tmpC == 2) {
					set_PWM(329.63);
					break;
				}
				else if(tmpC == 3) {
					set_PWM(349.23);
					break;
				}
				else if(tmpC == 4) {
					set_PWM(392.00);
					break;
				}
				else if(tmpC == 5) {
					set_PWM(440.00);
					break;
				}
				else if(tmpC == 6) {
					set_PWM(493.88);
					break;
				}
				else if(tmpC == 7) {
					set_PWM(523.25);
					break;
				}
				break;
			}
			break;
			
		default:
			Sound_State = WaitRise;
			break;
	}
}



int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x08; PORTB = 0x00;
	PWM_on();
	set_PWM(1);
    /* Replace with your application code */
    while (1) 
    {
		TickFct_Sound();
	
    }
}

