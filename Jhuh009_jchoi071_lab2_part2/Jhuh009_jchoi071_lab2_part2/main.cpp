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
    DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs
    unsigned char tmpA = 0x00;
    unsigned char cntavail;
    unsigned char loop;

    while(1)
    {
        tmpA = PINA & 0x0F;

        for (loop = 0; loop < 4; ++loop)
        {
            if (tmpA & 0x01 == 0x01)
            {
                cntavail += 1;
            }

            tmpA = tmpA >> 1;
        }
		if(cntavail == 0x04){
			cntavail += 128;	
		}
        PORTC = cntavail;

        cntavail = 0;
    }

    return 0;
}