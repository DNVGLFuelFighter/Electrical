/*
 * timer1.c
 *
 * Created: 3/25/2016 1:47:42 PM
 *  Author: Vilius
 */ 
#include "timer1.h"
#include <avr/interrupt.h>

void timer1_init( void) {
	unsigned int i;
	
	/* Set TCT1 to 0x01FF */
	TCNT1 = 0x1FF;
	/* Read TCNT1 into i */
	i = TCNT1;
	
	/* Use CTC mode */
	set_bit(TCCR1A, COM1A1);
	clear_bit(TCCR1A, COM1A0);
	
	/* Select correct clock */
	
}

unsigned int TIM16_ReadTCNT1( void) {
	unsigned char sreg;
	unsigned int i;
	/* Save global interrupt flag */
	sreg = SREG;
	/* Disable interrupts */
	_CLI(); // same as cli()??
	/* Read TCNT1 into i */
	i = TCNT1;
	/* Restore global interrupt flag */
	SREG = sreg;
	return i;
}

void TIM16_WriteTCNT1(unsigned int i) {
	unsigned char sreg;
	unsigned int i;
	/* Save global interrupt flag */
	sreg = SREG;
	/* DIsable interrupts */
	_CLI() // same as cli()?
	/* Set TCNT1 to i */
	TCNT1 = i;
	/* Restore global interrupt flag */
	SREG = sreg;
}

ISR(TIMER1_OVF_vect) {
	
	
}