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
	/* Set PE3 and PE4 as inputs */
	clear_bit(DDRE, DDE3);
	clear_bit(DDRE, DDE4);
	/* Set PE3 and PE4 as high (pull up res) */
	set_bit(PORTE, PE3);
	set_bit(PORTE, PE4);	
}

void sw_input( CAN_packet* p) {	
	/* Read left/right indicator */
	if(!test_bit(PINE, PE4))
		p->data[0] = (1<<2);
	else if (!test_bit(PINE, PE3))
		p->data[0] = (1<<3);
	else
		p->data[0] = 0;
	
	/* Read cruise control */
	//message.data[2] = test_bit();
	/* Read cruise control plus */
	//message.data[3] = get_cc_plus();
	/* Read cruise control minus */
	//message.data[4] = get_cc_plus();
	/* Read horn */
	//message.data[5] = test_bit();
	/* Read whiper */
	//message.data[6] = test_bit();
	/* Read indicator left */
	//message.data[7] = test_bit();
	/* Read indicator right */
	//message.data[8] = test_bit(); // a problem. We only got 8 bits
	//return message;
	//message.data[0] = 0;	
}

