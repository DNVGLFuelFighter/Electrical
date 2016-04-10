/*
 * timer0.c
 *
 * Created: 3/25/2016 1:31:06 PM
 *  Author: Vilius
 */ 

#include "timer0.h"

unsigned char OVF_CNT = 0;
int PERIOD_CNT = 0;
BOOL DIR_CHANGE = FALSE;

void timer0_init( void) {
	/* clk/1 */
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
		//printf("\r\nOVF_CNT = %d", OVF_CNT);
		OVF_CNT = 0;
		/* Wiper signal */
		set_bit(PORTB, PB4);
	}
	if(PERIOD_CNT >= 3124*10) {
		//printf("\r\nPERIOD_CNT value = %d", PERIOD_CNT);
		PERIOD_CNT = 0;
		DIR_CHANGE = TRUE;
	}
	else
		PERIOD_CNT++;
	sei();	
}