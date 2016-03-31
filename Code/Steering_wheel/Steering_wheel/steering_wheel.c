/*
 * steering_wheel.c
 *
 * Created: 3/29/2016 4:12:29 PM
 *  Author: Vilius
 */ 

#include "steering_wheel.h"


CAN_packet current;
CAN_packet updated;

void sw_init( void) {	
	/* Indicators init */
	/* Set PE3 and PE4 as inputs */
	clear_bit(DDRE, DDE3);
	clear_bit(DDRE, DDE4);
	/* Set PE3 and PE4 as high (pull up res) */
	set_bit(PORTE, PE3);
	set_bit(PORTE, PE4);	
	
	/* Whiper init */
	clear_bit(DDRE, DDE5);
	set_bit(PORTE, PE5);
	
	/* Horn init */
	clear_bit(DDRB, DDB4);
	set_bit(PORTB, PB4);
	
	/* Deadman */
	clear_bit(DDRB, DDB3);
	set_bit(PORTB, PB3);
	
	/* Speed potentiometer uses adc_init() */
	
	/* Cruise control */
	clear_bit(DDRB, DDB2);
	clear_bit(DDRB, DDB1);
	set_bit(PORTB, PB2);
	set_bit(PORTB, PB1);	
}

void sw_input( CAN_packet* p) {	
	/* Initialize data */
	p->data[0] = 0x00;
	/* Read right/left indicator */
	if(!test_bit(PINE, PE4))
		p->data[0] |= (1<<0);
	else if (!test_bit(PINE, PE3))
		p->data[0] |= (1<<1);	
	/* Read cruise control */
	if(!test_bit(PINB, PB2))
		p->data[0] |= (1<<2);
	else if(!test_bit(PINB, PB1))
		p->data[0] |= (1<<3);	
	/* Read horn */
	//if (!test_bit(PINB, PB4))
	p->data[0] |= (1<<4);
	/* Read whiper */
	//if (!test_bit(PINE, PE5))
	p->data[0] |= (1<<5);
	/* Read deadman */
	if (!test_bit(PINB, PB3))
		p->data[0] |= (1<<6);
}

