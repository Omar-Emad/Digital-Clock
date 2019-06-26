/*
 * IOports.c
 *
 *  Created on: 12/8/2018
 *      Author: Omar
 */

#include <avr/io.h>
#include <avr/interrupt.h>

void display_init(void)
{
	DDRA |= 0x0F;
	PORTA &= 0xF0;
}


void enable_init (void)
{
	DDRC |= 0x7F ;
	PORTC &= 0xC0;
	PORTC |= (1<<6);
}

void switch_init (void) // 2 switches to pause/Resume the clock and to reset it 
{
	DDRD &= ~ ((1<<2) | (1<<3));  //switch 1 and 2 are input reset and stop/pause
	PORTD |= (1<<2) | (1<<3);     // pull up res
	MCUCR = (MCUCR & ~1) | (1<<1) | ~(1<<2) | (1<<3);   // interrupt on falling edge
	GICR |= 0xF0;       // enable interrupt

	DDRB &= ~(1<<2);
	PORTB |= (1<<2);
	MCUCSR &= ~(1<<6);
}


void edit_init(void) // for edit the clock
{
	DDRD &= ~ 0x3;
	PORTD |= 0x03;
}
