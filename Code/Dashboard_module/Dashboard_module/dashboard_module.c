/*
 * dashboard_module.c
 *
 * Created: 4/5/2016 11:19:05 AM
 *  Author: Vilius
 */ 

#include "dashboard_module.h"

void db_init( void){
	/* Backlights init */
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
	
}

void db_input( CAN_packet *p) {
	
}