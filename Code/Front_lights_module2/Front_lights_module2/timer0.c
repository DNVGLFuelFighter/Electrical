/*
 * timer0.c
 *
 * Created: 3/25/2016 1:31:06 PM
 *  Author: Vilius
 */ 

#include "timer0.h"

int different;
BOOL ret;

CAN_packet current_msg;
CAN_packet updated_msg;

void timer0_init( void) {
	/* clk/1024 */
	set_bit(TCCR0A, CS02);
	set_bit(TCCR0A, CS00);
	/* Enable timer overflow */
	set_bit(TIMSK0, TOIE0);
	
	current_msg.id = ID_lightsFront;
	current_msg.length = 1;
	current_msg.data[0] = 0;
	updated_msg.id = ID_lightsFront;
	updated_msg.length = 1;
	updated_msg.data[0] = 0;
}

ISR(TIMER0_OVF_vect) {
	cli();
	get_light_status(&updated_msg);
	sei();
	
	different = memcmp(current_msg.data, updated_msg.data, current_msg.length);
	if(different) {
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