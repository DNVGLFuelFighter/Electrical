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

	/***********************************************************
	Printing fuction for the data received from the hall sensor
	***********************************************************/
// 	if(p->id == 10) {
// 		uint16_t data;
// 		data |= p->data[0] *256 + p->data[1];
// 		data = data/100;
// 		printf("\r\nSpeed - %u m/s", data);
// 	}
	/***********************************************************
	Printing fuction for the throttle input
	***********************************************************/
	/*if (p->id == 13) {*/
		printf("\r\nMessage ID - %d", p->id);
		printf("\r\nMessage length - %d", p->length);
		for(int i = 0; i < p->length; i++) {
			
			printf("\r\nData[%d] received - %u", i, p->data[i]);
		}
		printf("\n");
	/*}*/
}

void inits(void) {
	can_init();
	USART_init(MYUBRR, true);
	printf("\r\nReceiver initialized");
	set_bit(DDRB, PB6);
	clear_bit(PORTB, PB6);
}

int main(void) {
	inits();

	DDRB |= (1<<DDB5);
	PORTB |= (1<<PB5);
	
	BOOL ret;	
	ret = prepare_rx(0, 20, 0, CAN_messageRX);

	sei();
	
	CAN_packet message;
	char MOb = 14;
	message.id = ID_lightsFront;
	message.length = 1;
	message.data[0] = 0b000000;	// For message structure see front light module documentation
	
	while (true) {
		if(ret) {
			toggle_bit(DDRB, PB6);
		}	
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

ISR(USART0_RX_vect) {
	unsigned char c = USART_rx();
	printf("\r\n%c", c);
}



