/*
 * dashboard_module.c
 *
 * Created: 4/5/2016 11:19:05 AM
 *  Author: Vilius
 */ 

#include "dashboard_module.h"

void db_init( void){
	/* Backlights show init */
	/* Set PB3 as inputs */
	clear_bit(DDRB, DDB3);
	/* Set PB3 as high (pull up res) */
	set_bit(PORTB, PB3);
	
	/* Backlights drive init */
	clear_bit(DDRB, DDB0);
	set_bit(PORTB, PB0);
	
	/* Headlight near/far */
	clear_bit(DDRE, DDE5);
	clear_bit(DDRD, DDD0);
	set_bit(PORTE, PE5);
	set_bit(PORTD, PD0);
	
	/* Eyebrows init */
	clear_bit(DDRE, DDE4);
	set_bit(PORTE, PE4);
	
	/* Angel eyes init */
	clear_bit(DDRB, DDB1);
	set_bit(PORTB, PB1);
	
	/* Emergency light init */
	clear_bit(DDRE, DDE3);
	set_bit(PORTE, PE3);
	
	/* Fans initializes via adc_init() */	
}

void db_input( CAN_packet *p) {
	/* Initialize data */
	p->data[0] = 0x00;
	/* Read headlights near/far */
	if(!test_bit(PINE, PE5))
		p->data[0] |= (1<<0);
	else if (!test_bit(PIND, PD0))
		p->data[0] |= (1<<1);
	/* Read angel eyes */
	if(!test_bit(PINB, PB1))
		p->data[0] |= (1<<2);
	/* Read eyebrows */
	if(!test_bit(PINE, PE4))
		p->data[0] |= (1<<3);
	/* Read emergency light */
	if (!test_bit(PINE, PE3))
		p->data[0] |= (1<<4);
	/* Read backlights drive/show  */
	if (!test_bit(PINB, PB0))
		p->data[0] |= (1<<5);
	else if (!test_bit(PINB, PB3))
		p->data[0] |= (1<<6);
}