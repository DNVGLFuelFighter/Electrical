/*
 * Steering_wheel.c
 *
 * Created: 3/29/2016 3:58:34 PM
 * Author : Vilius
 */ 
#include "definitions.h"
#include "can.h"
#include "timer0.h"
#include "USART.h"
#include "steering_wheel.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

void inits(CAN_packet *current, CAN_packet* updated) {
	can_init();
	USART_init(MYUBRR, true);
	timer0_init();
	sw_init(current, updated);
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
	
	/* Make two CAN packets */
	CAN_packet current;
	CAN_packet updated;
	
	/* Initialize module */
	inits(&current, &updated);
	
	DDRB = 0x00;
	PORTB = 0xff;
    for(;;) {
		_delay_ms(100);
		/* Write input to CAN_packet "current" */
		updated = sw_input(); 
		/* Compare the two packets */
		different = memcmp(current.data, updated.data, 8);
		if (different) {
			/* Send a message with new data */
			ret = can_packet_send(1, &updated);
			current = updated;
		}
		if (ret) {
			printf("\r\nDead man's switch");
			ret = FALSE;
		}
		//TODO: if successfully read, stop sending!
		asm("sleep");;
	}
/*
	ret = prepare_rx(0, ID_steeringWheel, 0xff, response);
	sei();
	
	for(;;)
		asm("sleep");;
    return 0;
	*/
}


