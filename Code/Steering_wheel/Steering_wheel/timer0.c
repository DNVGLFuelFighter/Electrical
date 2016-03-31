/*
 * timer0.c
 *
 * Created: 3/25/2016 1:31:06 PM
 *  Author: Vilius
 */ 

#include "timer0.h"

void timer0_init( void) {
	/* clk/1024 */
	set_bit(TCCR0A, CS02);
	set_bit(TCCR0A, CS00);
	/* Enable timer overflow */
	set_bit(TIMSK0, TOIE0);
}

ISR(TIMER0_OVF_vect) {
	
	
}