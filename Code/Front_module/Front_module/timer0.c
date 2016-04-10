/*
 * timer0.c
 *
 * Created: 3/25/2016 1:31:06 PM
 *  Author: Vilius
 */ 

#include "timer0.h"

void timer0_init( void) {
	/* clk/64 */
	set_bit(TCCR0A, CS01);
	set_bit(TCCR0A, CS00);
	/* Enable timer overflow */
	set_bit(TIMSK0, TOIE0);
	sei();	
}

ISR(TIMER0_OVF_vect) {
	cli();
	if(HORN_ON) {		
		fm_horn(12.0);
		//_delay_us(1000);
		_delay_us(2500); //USE THIS VALUE -> high sound
	}
	fm_horn(0);
	sei();	
}