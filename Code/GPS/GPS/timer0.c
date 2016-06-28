/*
 * timer0.c
 *
 * Created: 3/25/2016 1:31:06 PM
 *  Author: Vilius
 */ 

#include "timer0.h"

BOOL TIMER0_SW_OVF = FALSE;
BOOL TIMER0_POWER_OVF = FALSE;
BOOL TIMER0_ESC_OVF = FALSE;

void timer0_init( void) {
	/*	CS02	CS01	CS00	Prescale
		0		0		0		No clock source(timer stopped)
		0		0		1		clkIO
		0		1		0		clkIO/8
		0		1		1		clkIO/64
		1		0		0		clkIO/256
		1		0		1		clkIO/1024
		1		1		0		clkIO/External clk on T0. Clk on falling edge
		1		1		1		clkIO/External clk on T0. Clk on rising edge
	*/
	
	/* clk/1024 */  //--> 32.768 ms
	set_bit(TCCR0A, CS02);
	set_bit(TCCR0A, CS00);
	/* Enable timer overflow */
	set_bit(TIMSK0, TOIE0);
}

ISR(TIMER0_OVF_vect) {
	cli();
	TIMER0_SW_OVF = TIMER0_SW_OVF + 1;
	TIMER0_POWER_OVF = TIMER0_POWER_OVF + 1;
	TIMER0_ESC_OVF = TIMER0_ESC_OVF + 1;
	sei();
}