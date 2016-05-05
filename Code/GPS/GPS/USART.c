/*
 * USART.c
 *
 * Created: 3/19/2016 2:56:17 PM
 *  Author: Vilius
 */ 

#include <avr/io.h>
#include "definitions.h"
#include "USART.h"

/*! Initialize the serial communication
	baud is the baud rate */
void USART_init(unsigned int baud, _Bool onOff) {
	/* Set baud rate */
	UBRR0H = (unsigned char) (baud>>8);
	UBRR0L = (unsigned char) baud;
	/* Set frame format: Asynchronous normal mode, 8data, no parity  1 stop bit */
	/*(0<<UMSEL0) | (0<<UPM0) | (0<<USBS0) | already set as Initial values */
	UCSR0C = (3<<UCSZ0);
	/* Enable receiver and transmitter */
	if (onOff) {
		set_bit(UCSR0B,	RXEN0);
		set_bit(UCSR0B, TXEN0);
	}
	set_bit(UCSR0B, RXCIE0);
	/*!	enables using fprint() instead of USART_tx_string() */
	fdevopen(USART_tx, USART_rx);
}

_Bool USART_tx(unsigned char data) {
	/* Wait for empty transmit buffer */
	while (!(UCSR0A & (1<<UDRE0)));
	/* Put data into buffer, sends the data */
	UDR0 = data;
	// T0D0 implement 
	return true;
}

unsigned char USART_rx(void) {
	/* Wait for data to be received */
	while (!(UCSR0A & (1<<RXC0)));
	/* Get and return received data from buffer */
	return UDR0;
}

_Bool USART_tx_string(char *text) {
	_Bool flag;
	while(*text) {
		flag = USART_tx(*text++);
	}
	return flag;
}
