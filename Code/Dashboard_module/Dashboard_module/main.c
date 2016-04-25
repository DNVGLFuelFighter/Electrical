/*
 * Dashboard_module.c
 *
 * Created: 4/5/2016 11:07:41 AM
 * Author : Vilius
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "definitions.h"
#include "dashboard_module.h"
#include "USART.h"
#include "can.h"

void inits();

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
	current_msg.id = ID_dashboard;
	current_msg.length = 2;
	updated_msg.id = ID_dashboard;
	updated_msg.length = 2;
	db_input(&current_msg);
	db_input(&updated_msg);
// 	adc_input(&current_msg);
// 	adc_input(&updated_msg);
// 	adc_sleep();
	
    for(;;) {
		/* Update one CAN_packet */
		db_input(&updated_msg);
// 		adc_init();
// 		adc_input(&updated_msg);
// 		adc_sleep();
		
		/* Compare the two packets */
		different = memcmp(current_msg.data, updated_msg.data, 8);
		if (different) {
			/* Send a message with new data */
			
			ret = can_packet_send(5, &updated_msg);
			current_msg = updated_msg;
		}
		//ret = can_packet_send(1, &updated_msg);
		
		if (ret) {
			set_bit(DDRB, PB7);
			ret = FALSE;
		} else {
			clear_bit(DDRB, PB7);
			
		}
		asm("sleep");
	}
	return 0;
}

void inits( void) {
	USART_init(MYUBRR, TRUE);
	can_init();
	db_init();
	printf("\r\nInitialization done");
	sei();
	set_bit(DDRB, PB6);
}

