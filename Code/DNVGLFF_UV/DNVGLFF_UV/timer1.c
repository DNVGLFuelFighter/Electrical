/*
 * timer1.c
 *
 * Created: 3/25/2016 1:47:42 PM
 *  Author: Vilius
 */ 
#include "timer1.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

BOOL T1_OVF_CNT = FALSE;

void timer1_init( void) {
	
	/* Set up timer with prescaler = 64 */
	set_bit(TCCR1B, CS11);
	set_bit(TCCR1B, CS10);
	
	/* Initialize counter */
	TCNT1 = 0;
	
	/* Enable overflow interrupt */
	set_bit(TIMSK1, TOIE1);
	
	/* Enable global interrupts*/
	sei();
	
	/* Initialize overflow variable */
	T1_OVF_CNT = 0;
	/*
	unsigned int i;
	// How to access the 16-bit timer registers 
	// Set TCT1 to 0x01FF 
	TCNT1 = 0x1FF;
	// Read TCNT1 into i 
	i = TCNT1;
	*/
	
	/* Use CTC mode */
	set_bit(TCCR1A, COM1A1);
	clear_bit(TCCR1A, COM1A0);
}

unsigned int TIM16_ReadTCNT1( void) {
	unsigned char sreg;
	unsigned int i;
	/* Save global interrupt flag */
	sreg = SREG;
	/* Disable interrupts */
	cli(); // same as cli()??
	/* Read TCNT1 into i */
	i = TCNT1;
	/* Restore global interrupt flag */
	SREG = sreg;
	return i;
}

void TIM16_WriteTCNT1(unsigned int i) {
	unsigned char sreg;
	/* Save global interrupt flag */
	sreg = SREG;
	/* DIsable interrupts */
	cli(); // same as cli()?
	/* Set TCNT1 to i */
	TCNT1 = i;
	/* Restore global interrupt flag */
	SREG = sreg;
}


ISR(TIMER1_OVF_vect) {
	printf(("\r\nTimer1 overflow"));
	if (IND_LEFT) {
		// toggle the left indicator
		if (get_ind_left())
		front_lights_turn_left(FALSE);
		else
		front_lights_turn_left(TRUE);
	} else
	front_lights_turn_left(FALSE);
	if (IND_RIGHT) {
		// toggle the right indicator
		if (get_ind_right())
		front_lights_turn_right(FALSE);
		else
		front_lights_turn_right(TRUE);
	} else
	front_lights_turn_right(FALSE);
}