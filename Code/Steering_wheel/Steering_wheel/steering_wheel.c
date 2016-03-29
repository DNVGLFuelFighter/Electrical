/*
 * steering_wheel.c
 *
 * Created: 3/29/2016 4:12:29 PM
 *  Author: Vilius
 */ 

#include "steering_wheel.h"


CAN_packet current;
CAN_packet updated;

void sw_init( CAN_packet *current, CAN_packet *updated) {
	*current = sw_input();
	*updated = sw_input();
}

CAN_packet sw_input( void) {
	CAN_packet message;
	message.id = ID_steeringWheel;
	message.length = 2;
	
	/* Read deadmans button */
	if(!test_bit(PINB, PB4)) {
		message.data[0] = 15;
	} else {
		message.data[0] = 0;
	}
	/* Read speed potentiometer */
	//message.data[1] = get_speed();
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
	//message.data[8] = test_bit();
	//return message;
	//message.data[0] = 0;
	message.data[1] = 1;
	message.data[2] = 2;
	message.data[3] = 3;
	message.data[4] = 4;
	message.data[5] = 5;
	message.data[6] = 6;
	message.data[7] = 7;
	message.data[8] = 8; //  a problem!
	
	return message;
}

