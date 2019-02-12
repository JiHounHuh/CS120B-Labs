/*
 * jhuh009_jchoi071_lab3_part1.c
 *
 * Created: 1/15/2019 12:50:35 PM
 * Author : Patrick Wumbo
 */ 

#include <avr/io.h>
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; //Configure port A's 8 pins as inputs, initialize to 1s
	DDRB = 0x00; PORTB = 0xFF; // Configure port B's 8 pins as inputs, initialize to 1s
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs, initialize to 0s
// 	unsigned char tmpA = 0x00;
// 	unsigned char tmpB = 0x00;
// 	unsigned char buttonA = 0x00;
// 	unsigned char buttonB = 0x00;
	unsigned short counter = 0x00;
	unsigned char place = 0x00;
	while(1) 
	{
		
// 		buttonA = PINA & 0x01;
// 		buttonB = PINB & 0x01;
// 		PINA >> 1;
// 		PINB >> 1;
// 		if(counter < 8)
// 		{
		for(counter = 0; counter < 8; counter++)
		{
			if(GetBit(PINA, counter)) {
				place++;
				//tmpA = SetBit(tmpA,1,0);
				//tmpA = SetBit(tmpA,0,1);
				
			}
			if(GetBit(PINB,counter)) {
				place++;
				// 			tmpB = SetBit(tmpB, 1, 0);
				// 			tmpB = SetBit(tmpB, 0, 1);
				
			}
			PORTC = place;	
		}
		
		//PINA >> 1;
		//PINB >> 1;
		
		place = 0;
			
		//}
		
	}
	/*
	DDRA = 0x00; PORTA = 0xFF; //Configure port A's 8 pins as inputs, initialize to 1s
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	unsigned char tmpB = 0x00;
	unsigned char button = 0x00;
	
	while(1)
	{
		
		button = PINA & 0x01;
		
		if(GetBit(button, 0)) {
			tmpB = SetBit(tmpB, 1, 0); //Set bit 1 to 0;
			tmpB = SetBit(tmpB, 0, 1); // Set bit 0 to 1;
		}
		else {
			tmpB = SetBit(tmpB,1,1); // Set bit 1 to 1
			tmpB = SetBit(tmpB, 0, 0); // Set bit 0 to 0
		}
		PORTB = tmpB;
	}
	*/
	/*
	DDRA = 0x00; PORTA = 0xFF; //Configure port A's 8 pins as inputs, initialize to 1s
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	unsigned char tmpB = 0x00;
	unsigned char button = 0x00;
	*/
    /* Replace with your application code */
	/*
    while (1) 
    {
		button = PINA & 0x01;
		// Perform Computation
		// if PA0 is 1, set PB1PB0 = 01, else 10
		if (button == 0x01) {
			tmpB = (tmpB & 0xFC) | 0x01; // Sets tmpB to bbbbbb01
		}
		else {
			tmpB = (tmpB & 0xFC) | 0x02; // Sets tmpB to bbbbbb10
		}
	}
	*/
	
}

