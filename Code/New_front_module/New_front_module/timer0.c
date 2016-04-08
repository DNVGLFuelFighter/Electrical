/*
 * timer0.c
 *
 * Created: 3/25/2016 1:31:06 PM
 *  Author: Vilius
 */ 

#include "timer0.h"

unsigned char OVF_CNT = 0;

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
	if(OVF_CNT < 100){
		OVF_CNT++;
	} else {
		OVF_CNT = 0;
	}
	sei();	
}