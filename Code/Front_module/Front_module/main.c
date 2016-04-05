/*
 * Front_module.c
 *
 * Created: 3/31/2016 10:07:27 AM
 * Author : Vilius
 */ 

#include "definitions.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "can.h"
#include "USART.h"
#include "front_module.h"
#include "timer0.h"


void inits( void) {
	USART_init(MYUBRR, true);
	can_init();
	fm_init();
	timer0_init();

	printf("\r\nInitialization done");
	set_bit(DDRB, PB6);
}

int main(void) {
	int different;
	BOOL ret;
	CAN_packet msg_old;
	CAN_packet msg_new;
	/* Initialize CAN packets */
	msg_old.id = ID_brakes;
	msg_old.length = 1;
	msg_new.id = ID_brakes;
	msg_new.length = 1;
	
	inits();
	prepare_rx(1, ID_steeringWheel, 0xff, fm_handler);
	sei();
	
	fm_brake(&msg_old);
	fm_brake(&msg_new);
    for(;;) {
		//_delay_ms(50);
		fm_brake(&msg_new);
		//printf("\r\nMsg_new.data[0] - %u", msg_new.data[0]);
		different = memcmp(msg_old.data, msg_new.data, 8);
		
		if(different){
			//printf("\r\n Brakes detected");
			ret = can_packet_send(0, &msg_new);
			msg_old = msg_new;
		} else
			ret = FALSE;
		if (ret)
			set_bit(DDRB, PB7);
		else
			clear_bit(DDRB, PB7);
		
		asm("sleep");;
	}
	return 0;
}

