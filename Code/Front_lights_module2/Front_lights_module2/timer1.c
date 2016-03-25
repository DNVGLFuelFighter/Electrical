/*
 * timer1.c
 *
 * Created: 3/25/2016 1:47:42 PM
 *  Author: Vilius
 */ 
#include "timer1.h"

void timer1_init( void) {
	unsigned int i;
	
	/* Set TCT1 to 0x01FF */
	TCNT1 = 0x1FF;
	/* Read TCNT1 into i */
	i = TCNT1;
}

unsigned int TIM16_ReadTCNT1( void) {
	unsigned char sreg;
	unsigned int i;
	/* Save global interrupt flag */
	sreg = SREG;
	/* Disable interrupts */
	cli();
	/* Read TCNT1 into i */
	i = TCNT1;
	/* Restore global interrupt flag */
	SREG = sreg;
}

ISR(TIMER1_OVF_vect) {
	
	
}