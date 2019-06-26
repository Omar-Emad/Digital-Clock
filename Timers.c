/*
 * Timers.c
 *
 *  Created on: 12/8/2018
 *      Author: Omar
 */

#include <avr/io.h>

void timer0_init (void)
{
	SREG |=(1<<7);

	OCR0 = 140;        // 156 * .000001 * 64 = .01 = 10 ms
	TIMSK |= 0x02;     // enable interrupt
	TCNT0 =0;         // clear counter
	TCCR0 = 0x8B ;     // 1000_1011 =>  prescaler 64 => CTC
}

void timer1_init (void)
{
	OCR1A = 12500;
	TIMSK |= (1<<4);
	TCNT1 = 0;
	TCCR1A = 0x08;
	TCCR1B = 0x0B;
}
