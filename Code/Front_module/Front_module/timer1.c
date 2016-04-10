/*
 * timer1.c
 *
 * Created: 4/8/2016 3:08:13 PM
 *  Author: Vilius
 */ 

#include "timer1.h"

//BOOL COMPB = FALSE;
int DIR_CNT = 0;

void timer1_init( void) {
	set_bit(TCCR1A, COM1B1);
	set_bit(TCCR1A, COM1B0);
	set_bit(TCCR1A, WGM11);
	set_bit(TCCR1A, WGM10);
	set_bit(TCCR1B, WGM13);
	set_bit(TCCR1B, WGM12);
	
	uint16_t max_cnt = 19999;
	uint16_t duty = max_cnt - 1024;
	OCR1A = max_cnt;
	OCR1B = duty;
	
	/* Enable output compare B match interrupt p. 142-3*/
	set_bit(TIMSK1, OCIE1B);
	
	set_bit(DDRB, DDB5);
	set_bit(DDRB, DDB4);
	
	set_bit(TCCR1B, CS11);
}

ISR(TIMER1_COMPB_vect) {
	//COMPB = TRUE;	
	DIR_CNT++;
	fm_wiper_routine();
}