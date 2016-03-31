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

void inits( void) {
	USART_init(MYUBRR, true);
	can_init();
	
	fm_init();
	printf("\r\nInitialization done");
}

int main(void)
{
	BOOL ret;
	
	inits();
	
	CAN_packet message;
	message.id = ID_brakes;
	message.length = 1;
	prepare_rx(0, ID_steeringWheel, 0xFF, fm_handler);
	sei();
	
    for(;;) {
		_delay_ms(100);
		fm_brake(&message);
		if(message.data[0] == 1){
			set_bit(DDRB, PB5);
			printf("\r\n Brakes detected");
			ret = can_packet_send(0, &message);
		}
		else
			clear_bit(DDRB, PB5);
		asm("sleep");;
	}
	return 0;
}

