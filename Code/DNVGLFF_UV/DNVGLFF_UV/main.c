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
	//USART_tx_string("\n\rMessage received!");
	toggle_bit(DDRB, PB6);
	
	int id = p->id;	
	printf("\r\nMessage ID - %d", p->id);
	printf("\r\nMessage length - %d", p->length);
	printf("\r\nData[0] received - %u", p->data[0]);
	printf("\r\nData[1] received - %u", p->data[1]);
	printf("\r\nData[2] received - %u", p->data[2]);
	printf("\r\nData[3] received - %u", p->data[3]);
	printf("\r\nData[4] received - %u", p->data[4]);
	printf("\r\nData[5] received - %u", p->data[5]);
	printf("\r\nData[6] received - %u", p->data[6]);
	printf("\r\nData[7] received - %u", p->data[7]);
}

void inits(void) {
	can_init();
	USART_init(MYUBRR, true);
	printf("\r\nInitialization complete");
}

int main(void)
{
	inits();

	DDRB |= (1<<DDB5);
	PORTB |= (1<<PB5);
	
	BOOL ret;	
	ret = prepare_rx(1, 20, 0xff, CAN_messageRX);

	sei();
	
	
	
	CAN_packet message;
	char MOb = 14;
	message.id = ID_lightsFront;
	message.length = 1;
	message.data[0] = 0b000000;	// For message structure see front light module documentation
	
	while (true) {
		
		_delay_ms(100);
// 		
// 		if (can_packet_send(MOb, &message)){
// 			USART_tx_string("\n\rThe message was put on the can bus");
// 			toggle_bit(DDRB, PB6);
// 			clear_bit(DDRB, PB5);
// 		} else {
// 			USART_tx_string("\n\rThe message is already on the bus");
// 			toggle_bit(DDRB, PB5);
// 			clear_bit(DDRB, PB6);
// 		}
    }
}



