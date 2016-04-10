/*
 * Steering_wheel.c
 *
 * Created: 3/29/2016 3:58:34 PM
 * Author : Vilius
 */ 
#include "definitions.h"
#include "can.h"
#include "USART.h"
#include "steering_wheel.h"
#include "adc.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

void inits( void) {
	can_init();
	USART_init(MYUBRR, true);
	sw_init();
	adc_init();
	printf("\r\nInitialization complete");
	sei();
	set_bit(DDRB, PB6);
}

int main(void)
{
	int different;
	BOOL ret;
	ret = FALSE;
		
	/* Make two CAN packets */
	CAN_packet current_msg;
	CAN_packet updated_msg;
	
	/* Initialize module and packets*/
	inits();
	
	/* Initialize packets */
	current_msg.id = ID_steeringWheel;
	current_msg.length = 6;
	updated_msg.id = ID_steeringWheel;
	updated_msg.length = 6;
	sw_input(&current_msg);
	sw_input(&updated_msg);
	adc_input(&current_msg);
	adc_input(&updated_msg);
	adc_sleep();
	
    for(;;) {
		
		/* Update one CAN_packet */
		sw_input(&updated_msg); 
		adc_init();
		adc_input(&updated_msg);
		adc_sleep();
		/* Compare the two packets */
		different = memcmp(current_msg.data, updated_msg.data, 8);
		if (different) {			
			/* Send a message with new data */
			printf("\r\nData sent - %d", updated_msg.data[1]);
			ret = can_packet_send(1, &updated_msg);
			current_msg = updated_msg;
		}
		//ret = can_packet_send(1, &updated_msg);
		
		if (ret) {
			printf("\r\nMessage sent");
			set_bit(DDRB, PB7);
			ret = FALSE;
		} else
			clear_bit(DDRB, PB7);
 		//TODO: if successfully read, stop sending!
		asm("sleep");;
	}
}