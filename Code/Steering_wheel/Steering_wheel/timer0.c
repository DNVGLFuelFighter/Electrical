/*
 * timer0.c
 *
 * Created: 3/25/2016 1:31:06 PM
 *  Author: Vilius
 */ 

#include "timer0.h"

/* Init variables */
int diff_buttons;
int diff_speed;
BOOL ret;
BOOL turned;
/* Make two CAN packets */
CAN_packet current_msg;
CAN_packet updated_msg;


void timer0_init( void) {
	/* clk/1024 */
	set_bit(TCCR0A, CS02);
	set_bit(TCCR0A, CS00);
	/* Enable timer overflow */
	set_bit(TIMSK0, TOIE0);
	
	current_msg.id = ID_steeringWheel;
	current_msg.length = 2;
	current_msg.data[0] = 0;
	current_msg.data[1] = 0;
	/*current_msg.data[2] = 0;*/
	updated_msg.id = ID_steeringWheel;
	updated_msg.length = 2;
	updated_msg.data[0] = 0;
	updated_msg.data[1] = 0;
	/*current_msg.data[2] = 0;*/
	ret = FALSE;
	/*turned = TRUE;*/
}

ISR(TIMER0_OVF_vect) {
	/* Update one CAN_packet */
	cli();
	sw_input(&updated_msg);	
	adc_init();
	adc_input(1, &updated_msg);
	//adc_input(2, &updated_msg);
	adc_sleep();
// 	if(((test_bit(updated_msg.data[0], 0)) || (test_bit(updated_msg.data[0], 1))) && (updated_msg.data[2] > 30) && !turned)
// 		turned = TRUE;
// 	if(turned && (updated_msg.data[2] < 10)) {
// 		turned = FALSE;
// 		clear_bit(updated_msg.data[0], 0);
// 		clear_bit(updated_msg.data[0], 1);
// 	}		
	sei();
	/* Compare the two packets */
	diff_buttons = memcmp(&current_msg.data[0], &updated_msg.data[0], 1);
	diff_speed = memcmp(&updated_msg.data[1], &current_msg.data[1], 1);

	if (((diff_speed < 100) &&(abs(diff_speed) > 3)) || abs(diff_buttons)) {
// 		printf("\r\nCurrent ID %d, d[0] %d, d[1] %d", current_msg.id, current_msg.data[0], current_msg.data[1]);
// 		printf("\r\nUpdated ID %d, d[0] %d, d[1] %d", updated_msg.id, updated_msg.data[0], updated_msg.data[1]);
// 		printf("\r\ndiff_buttons - %d", diff_buttons);
// 		printf("\r\ndiff_speed - %d\n", diff_speed);
		/* Send a message with new data */
		ret = can_packet_send(0, &updated_msg);
		current_msg = updated_msg;	
	}
// 	printf("\r\nMessage ID - %d", updated_msg.id);
// 	printf("\r\nMessage length - %d", updated_msg.length);
// 	for(int i = 0; i < 3; i++)
// 	printf("\r\nData[%d] received - %u", i, updated_msg.data[i]);
// 	printf("\n");
	if (ret) {
		set_bit(DDRB, PB7);
		ret = FALSE;
	} else
		clear_bit(DDRB, PB7);
}