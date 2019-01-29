/*
 * sample 1.c
 *
 * Created: 1/29/2019 12:32:36 PM
 * Author : Patrick Wumbo
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;


void TimerOn() {
	TCCR1B = 0x0B;
	// bit 3 = 0; CTC mode (Clear Timer on Compare)
	// bit2bit1bit0 = 011: pre-scalar / 64
	// 00001011: 0x0B
	// so 8 MHz clock or 8,000,000 / 64 = 125,000 tick/s
	// Thus TCNT1 register will count at 125,000 tick/s
	
	// AVR output compare register  OCR1A
	OCR1A = 125;
	
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt
	
	// Initialize avr counter 
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	
	//Enable global interrupts
	SREG |= 0x80;	
}

void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0 = 000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}


int main(void)
{
    DDRB = 0xFF;
	PORTB = 0x00;
	TimerSet(1000);
	TimerOn();
	unsigned char tmpB = 0x00;
    while (1) 
    {
		tmpB = ~tmpB; // Toggle PORTB. This is a bad programming method
		PORTB = tmpB;
		while (!TimerFlag); // Wait 1 second
		TimerFlag = 0;		
    }
}

