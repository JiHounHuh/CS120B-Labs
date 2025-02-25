/*
 * jhuh009_jchoi071_lab2_part2.cpp
 *
 * Created: 1/12/2019 4:36:37 PM
 * Author : Patrick Wumbo
 */ 

#include <avr/io.h>


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	
	unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
	unsigned char tmpB = 0x00; // Temporary variable to hold the value of B

	while (1)
	{
		tmpA = PINA & 0x03;
		if (tmpA == 0x01)
		{
			tmpB = (tmpB & 0xFE) | 0x01;
		}
		else
		{
			tmpB = (tmpB & 0xFE) | 0x00;
		}
		
		PORTB = tmpB;
	}
	
	return 0;
}

