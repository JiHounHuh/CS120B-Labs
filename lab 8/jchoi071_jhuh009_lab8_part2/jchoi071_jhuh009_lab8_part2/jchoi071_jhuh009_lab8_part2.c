/*
 * jchoi071_jhuh009_lab8_part_2.c
 *
 * Created: 2/5/2019 2:16:40 PM
 * Author : Patrick Wumbo
 */ 

#include <avr/io.h>


void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}



int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	ADC_init();
	
    /* Replace with your application code */
    while (1) 
    {
		//unsigned short x = ADC;  // Value of ADC register now stored in variable x
		
		unsigned short my_short = ADC;
		unsigned char my_char = (char)my_short; // my_char = 0xCD
		my_char = (char)(my_short);
		PORTB = my_short;
		my_char = (char)(my_short >> 8); // my_char = 0xBC
		PORTD = my_char;
    }
}

