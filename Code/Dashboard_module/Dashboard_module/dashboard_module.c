/*
 * dashboard_module.c
 *
 * Created: 4/5/2016 11:19:05 AM
 *  Author: Vilius
 */ 

#include "dashboard_module.h"

int num_rear_design = 0;
const int MAX_NUM_REAR_DESIGN = 2;
BOOL design_change = FALSE;
BOOL design_temp = FALSE;

void db_init( void){
	/* Backlights show init */
	clear_bit(DDRF, DDF3);
	set_bit(PORTF, PF3);
	
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
	
	/* Design change init */
	clear_bit(DDRB, DDB3);
	set_bit(PORTB, PB3);
	
	/* Angel lights and eyebrows init */
	clear_bit(DDRE, DDE4);
	set_bit(PORTE, PE4);
	
	/* Emergency light init */
	clear_bit(DDRE, DDE3);
	set_bit(PORTE, PE3);
}

void db_input( CAN_packet *p) {
	/* Initialize data */
	p->data[0] = 0;
	p->data[1] = 0;
	/* Read front lights near/far */
	if(!test_bit(PINE, PE5)) {
		p->data[0] |= (1<<0);
	}
	else if (!test_bit(PIND, PD0)) {
		p->data[0] |= (1<<1);
	}
	/* Read fans  */
	if(!test_bit(PINB, PB0)) {
		p->data[0] |= (1<<5);
	}
	/* Read emergency light */
	if (!test_bit(PINE, PE3)) {
		p->data[0] |= (1<<4);
	}
	/* Read angel and eyebrows  */
	if (!test_bit(PINE, PE4)) {
		p->data[0] |= (1<<2);
		p->data[0] |= (1<<3);	
	}
	/* Read rear drive/show */
	if (!test_bit(PINB, PB1)) {
		p->data[0] |= (1<<6);
	} else if (!test_bit(PINF, PF3)) {
		p->data[0] |= (1<<7);
	}
	/* Change rear show/drive design */		
	design_temp = !test_bit(PINB, PB3);
	if(design_temp == TRUE && design_change == FALSE) {
		design_change = TRUE;
		if(num_rear_design >= MAX_NUM_REAR_DESIGN) {
			p->data[1] = 0;
			num_rear_design = 0;
		}
		else {
			num_rear_design = num_rear_design+1;
		}
	} else {
		design_change = design_temp;
	}
	p->data[1] = num_rear_design;
}