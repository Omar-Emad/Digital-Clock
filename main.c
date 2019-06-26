/*
 * main.c
 *
 *  Created on: 12/8/2018
 *      Author: Omar
 */

#include "IOports.h"
#include "Timers.h"
#include "std_types.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define MINUTES_COUNT 1
#define HOURS_COUNT 2

typedef enum up_down { up , down } up_down;


boolean num [6] = {0};
uint8 displayNum =0;
up_down status = up;

ISR (TIMER1_COMPA_vect) //each 1 s increment the timer
{
	switch (status)
	{
	case up :
		if(num[0]==9)
		{
			num[0] =0;
			num[1]++;
		}
		else
			num[0]++;

		if(num[1]==6)
		{
			num[1]=0;
			num[2]++;
		}

		if(num[2]==10)
		{
			num[2]=0;
			num[3]++;
		}

		if(num[3]==6)
		{
			num[3]=0;
			num[4]++;
		}

		if (num[4]==4 && num[5]==2)
		{
			num[4]=num[5]=0;
		}
		else if( num[4]==10 )
		{
			num[4]=0;
			num[5]++;
		}
		break;

	case down :
		if(num[0]==0)
		{
			num[0] =9;
			num[1]--;
		}
		else
			num[0]--;

		if(num[1]==255)
		{
			num[1]=5;
			num[2]--;
		}

		if(num[2]==255)
		{
			num[2]=9;
			num[3]--;
		}

		if(num[3]==255)
		{
			num[3]=5;
			num[4]--;
		}

		if(num[4]==255 && num[5]==0)
		{
			num[4]=3;
			num[5]=2;
		}
		else if(num[4]==255)
		{
			num[4]=9;
			num[5]--;
		}
		break;
	}
}

uint8 count=0;

ISR (TIMER0_COMP_vect) // each 10 ms switch the displays
{

	PORTC &= ~0x3F;  // 11000000
	PORTC |=(1<<count); // enable the next display
	PORTC |= (1<<6); // tp light the two dots on the diplay for example 05:16:10
	PORTA = (PORTA & ~0x0F) | (num[count]);  // print the num on the next display
	if(count==2 || count==4)
			PORTC &= ~(1<<6);
	count++;
	if(count==6) // only i have 6 displays from 0 to 5
		count=0;
}

ISR (INT0_vect)  // to reset the timer
{
	for(int i=0;i<6;i++)
	{
		num[i]=0;
	}
	TCNT1 = 0;
}

ISR (INT1_vect) // to toggle between pause/Resume
{
	TCNT1 = 0;          //initial the counter
	TCCR1B ^= 0x03;     // toggle the clock source (000 timer disable) and (011 prescaler = 64)
}

ISR (INT2_vect)   // to toggle between up and down counts
{
	TCNT1 = 0;
	status ^= 1;   //toggle the status
}



//============= main=============================================
int main()
{
	SREG |= (1<<7);     // enable global interrupt
	DDRC |= (1<<7);
	display_init();     // initial the 4 pins of the decoder
	enable_init();      // initial the 6 pins of the enable
	switch_init();      // initial the reset switch
	edit_init();

	timer0_init();      //display the number each 10 ms
	timer1_init();      // increment the clock each 1 sec


	DDRB |= 1;
	for(;;)  //  2 switches to edit the timer first one  to edit the hours and second to edit the minutes 
	{ 
		if( !(PIND & MINUTES_COUNT) )
		{
			if(num[2]==9)
			{
				num[2]=0;
				num[3]++;
			}
			else
				num[2]++;
			if(num[3]==6)
				num[3]=0;
		}
		else if( !(PIND & HOURS_COUNT) )
		{
			if(num[4]==9)
			{
				num[4]=0;
				num[5]++;
			}
			else
				num[4]++;
			if(num[5]==2 && num[4]==4)
			{
				num[5]=num[4]=0;
			}
		}

		_delay_ms(150);
	}


}



