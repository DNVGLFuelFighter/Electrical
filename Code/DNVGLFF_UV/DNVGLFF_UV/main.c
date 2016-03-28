/*
 * DNVGLFF_UV.c
 *
 * Created: 3/19/2016 12:11:38 PM
 * Author : Vilius
 */ 
#include <avr/interrupt.h>
#include "definitions.h"
#include "can.h"
#include "USART.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "LED.h"

void CAN_messageRX(CAN_packet *p, unsigned char mob) {		
	USART_tx_string("\n\rMessage received!");
	toggle_bit(DDRB, PB6);
	_delay_ms(100);
	int id = p->id;	

	printf("ID - %d", id);
}

void inits(void) {
	can_init();
	USART_init(MYUBRR, true);
	printf("\r\nInitialization complete");
}

int main(void)
{
	inits();
/*
	DDRB |= (1<<DDB5);
	PORTB |= (1<<PB5);
	
	BOOL ret;	
	ret = prepare_rx(1, 241, 241, CAN_messageRX);

	sei();
	
	for(;;) 
		asm("sleep");;
	return 0;
	*/
	
	CAN_packet message;
	char MOb = 14;
	message.id = ID_lightsFront;
	message.length = 1;
	message.data[0] = 0b000000;	// For message structure see front light module documentation
	
	while (true) {
		
		_delay_ms(3000);
		
		if (can_packet_send(MOb, &message)){
			USART_tx_string("\n\rThe message was put on the can bus");
			toggle_bit(DDRB, PB6);
			clear_bit(DDRB, PB5);
		} else {
			USART_tx_string("\n\rThe message is already on the bus");
			toggle_bit(DDRB, PB5);
			clear_bit(DDRB, PB6);
		}
    }
}



