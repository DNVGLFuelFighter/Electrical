/*
 * USART.h
 *
 * Created: 3/19/2016 2:58:09 PM
 *  Author: Vilius
 */ 


#ifndef USART_H_
#define USART_H_

/*! Initialize the USART module. "ubbr" is MYUBBR in "definitions.h",
	while the boolean value turns the initialization on and off */
void USART_init(unsigned int ubbr, _Bool);

/*! Send one char over the serial port. Used in USART_tx_string.
	Returns positive when the transmission is done */
_Bool USART_tx(unsigned char data);

/*! Send a string over the serial port */
_Bool USART_tx_string(char* text);

/*! Receive the data over the serial port */
unsigned char USART_rx(void);

#endif /* USART_H_ */
