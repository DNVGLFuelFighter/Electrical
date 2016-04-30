/*
 * timer0.c
 *
 * Created: 3/25/2016 1:31:06 PM
 *  Author: Vilius
 */ 

#include "timer0.h"

/* Init variables */
int different;
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
	
	/* Initialize created CAN packets */
	current_msg.id = ID_brakes;
	current_msg.length = 1;
	current_msg.data[0] = 0;
	updated_msg.id = ID_brakes;
	updated_msg.length = 1;
	updated_msg.data[0] = 0;
	ret = FALSE;
}

ISR(TIMER0_OVF_vect) {
	/* Update one CAN_packet */
	cli();
	if(test_bit(PINE, PE5)) {
		updated_msg.data[0] = 1;
	}
	sei();
	/* Compare the two packets */
	different = memcmp(&current_msg.data[0], &updated_msg.data[0], 1);
	if (different) {
// 		printf("\r\nCurrent ID %d, d[0] %d, d[1] %d", current_msg.id, current_msg.data[0], current_msg.data[1]);
// 		printf("\r\nUpdated ID %d, d[0] %d, d[1] %d", updated_msg.id, updated_msg.data[0], updated_msg.data[1]);
// 		printf("\r\ndiff_buttons - %d", diff_buttons);
// 		printf("\r\ndiff_speed - %d\n", diff_speed);
		/* Send a message with new data */
		ret = can_packet_send(0, &updated_msg);
		current_msg = updated_msg;	
	}
	if (ret) {
		set_bit(DDRB, DDB7);
		ret = FALSE;
	} else
	clear_bit(DDRB, DDB7);
}