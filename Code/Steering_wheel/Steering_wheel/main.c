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
}

void response(CAN_packet* p, unsigned int mob) {
	(void)mob;
	printf("\r\nReceived");
}

int main(void)
{
	int different;
	BOOL ret;
	ret = FALSE;
		
	/* Make two CAN packets */
	CAN_packet current;
	CAN_packet updated;
	
	/* Initialize module and packets*/
	inits();
	
	/* Initialize packets */
	current.id = ID_steeringWheel;
	current.length = 1;
	updated.id = ID_steeringWheel;
	updated.length = 1;
	sw_input(&current);
	sw_input(&updated);
	adc_input(SPEED, &current);
	adc_input(SPEED, &updated);
	adc_sleep();
	
    for(;;) {
		_delay_ms(100);
		/* Update one CAN_packet */
		sw_input(&updated); 
		adc_init();
		adc_input(FANS, &updated);
		adc_sleep();
		/* Compare the two packets */
		different = memcmp(current.data, updated.data, 8);
		if (different) {
			/*printf("\r\nCurrent data[0] - %u",current.data[0]);
			printf("\r\nCurrent speed - %u",current.data[1]);
			printf("\r\nUpdated data[0] - %u",updated.data[0]);
			*/
			/* Send a message with new data */
			ret = can_packet_send(1, &updated);
			current = updated;
		}
		if (ret) {
			printf("\r\nMessage sent");
			ret = FALSE;
		}
 		//TODO: if successfully read, stop sending!
		asm("sleep");;
	}
}