/*
 * dashboard_module.c
 *
 * Created: 4/5/2016 11:19:05 AM
 *  Author: Vilius
 */ 

#include "dashboard_module.h"

void db_init( void){
	/* Backlights show init */
	clear_bit(DDRE, DDE4);
	set_bit(PORTE, PE4);
	
	/* Backlights drive init */
	clear_bit(DDRB, DDB1);
	set_bit(PORTB, PB1);
	
	/* Fans init */
	clear_bit(DDRB, DDB0);
	set_bit(PORTB, PB0);
	
	/* Headlight near/far */
	clear_bit(DDRE, DDE5);
	clear_bit(DDRD, DDD0);
	set_bit(PORTE, PE5);
	set_bit(PORTD, PD0);
	
	/* Angel and eyebrows init */
	clear_bit(DDRB, DDB3);
	set_bit(PORTB, PB3);
	
	/* Emergency light init */
	clear_bit(DDRE, DDE3);
	set_bit(PORTE, PE3);
}

void db_input( CAN_packet *p) {
	/* Initialize data */
	p->data[0] = 0x00;
	/* Read front lights near/far */
	if(!test_bit(PINE, PE5))
		p->data[0] |= (1<<0);
	else if (!test_bit(PIND, PD0))
		p->data[0] |= (1<<1);
	/* Read fans  */
	if(!test_bit(PINB, PB3))
		p->data[0] |= (1<<5);
	/* Read emergency light */
	if (!test_bit(PINE, PE3))
		p->data[0] |= (1<<4);
	/* Read angel and eyebrows  */
	if (!test_bit(PINB, PB0)) {
		p->data[0] |= (1<<2);
		p->data[0] |= (1<<3);	
	}
	/* Read rear drive/show */
	if (!test_bit(PINB, PB1))
		p->data[0] |= (1<<6);
	else if (!test_bit(PINE, PE4))
		p->data[0] |= (1<<7);
}