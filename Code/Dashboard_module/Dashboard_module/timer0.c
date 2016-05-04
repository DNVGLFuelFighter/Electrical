/*
 * timer0.c
 *
 * Created: 3/25/2016 1:31:06 PM
 *  Author: Vilius
 */ 

#include "timer0.h"

/* Init variables */
int different;
int diff_speed;
BOOL ret;
/* Make two CAN packets */
CAN_packet current_msg;
CAN_packet updated_msg;


void timer0_init( void) {
	/* clk/1024 */
	set_bit(TCCR0A, CS02);
	set_bit(TCCR0A, CS00);
	/* Enable timer overflow */
	set_bit(TIMSK0, TOIE0);
	
	current_msg.id = ID_dashboard;
	current_msg.length = 1;
	current_msg.data[0] = 0;
	updated_msg.id = ID_dashboard;
	updated_msg.length = 1;
	updated_msg.data[0] = 0;
	ret = FALSE;
}

ISR(TIMER0_OVF_vect) {
	/* Update one CAN_packet */
	cli();
	db_input(&updated_msg);
	sei();
	/* Compare the two packets */
	different = memcmp(current_msg.data, updated_msg.data, current_msg.length);
	if (different) {
		/* Send a message with new data */
		ret = can_packet_send(1, &updated_msg);
		current_msg = updated_msg;
	}
	if (ret) {
		set_bit(DDRB, PB7);
		ret = FALSE;
	} else
		clear_bit(DDRB, PB7);
}