/*
 * USART.c
 *
 * Created: 02.07.2013 17:52:25
 *  Author: Ole
 */ 

#include "definitions.h"
#include "USART.h"

void USART0_Init (unsigned int baud){
	/*
	Set baud rate
	*/
	UBRR0H = (	unsigned char	) (baud>>8);
	UBRR0L = (	unsigned char	) baud;
	
	/*
	Set frame format: 8data, no parity & 1 stop bit
	*/
	UCSR0C = (0<<UMSEL0) | (0<<UPM0) | (3<<UCSZ00);
	/*
	Enable receiver and transmitter
	*/
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	fdevopen(uart_putc0, USART0_Receive);
}

void USART1_Init (unsigned int baud){
	/*
	Set baud rate
	*/
	UBRR1H = (	unsigned char	) (baud>>8);
	UBRR1L = (	unsigned char	) baud;
	
	/*
	Set frame format: 8data, no parity & 1 stop bit
	*/
	UCSR1C = (0<<UMSEL1) | (0<<UPM1) | (3<<UCSZ10);
	/*
	Enable receiver and transmitter
	*/
	UCSR1B = (1<<RXEN1) | (1<<TXEN1);
// 	fdevopen(uart_putc1, USART1_Receive);
}

void uart_putc0 (unsigned char data){
	/*
	Wait for empty transmit buffer
	*/
	while( !( UCSR0A & (1<<UDRE0)));
	/*
	Put data into buffer, sends the data
	*/
	UDR0 = data;
}

void uart_putc1 (unsigned char data){
	/*
	Wait for empty transmit buffer
	*/
	while( !( UCSR1A & (1<<UDRE1)));
	/*
	Put data into buffer, sends the data
	*/
	UDR1 = data;
}

void uart_puts0(const char *s )
{
	while (*s)
	uart_putc0(*s++);

}

void uart_puts1(const char *s )
{
	while (*s)
	uart_putc1(*s++);

}

unsigned char USART0_Receive (void)
{
	/*
	Wait for data to be received
	*/
	while	( ! (UCSR0A & (1<<RXC0)));
	/*
	Get and return received data from buffer
	*/
	return	UDR0;
}

unsigned char USART1_Receive (void)
{
	/*
	Wait for data to be received
	*/
	while	( ! (UCSR1A & (1<<RXC1)));
	/*
	Get and return received data from buffer
	*/
	return UDR1;
}

unsigned char uart_receive0(char *str, unsigned char size, unsigned char endChar)
{
	unsigned char i = 0;
	if (size == 0)
	{
		return 0;
	}
	
	while (i < size - 1)
	{
		unsigned char c = USART0_Receive();
		if (c == endChar) break;
		str[i] = c;
		i++;
	}
	return i + 1;
}

unsigned char uart_receive1(char *str, unsigned char size, unsigned char endChar)
{
	unsigned char i = 0;
	if (size == 0)
	{
		return 0;
	}
	
	while (i < size - 1)
	{
		unsigned char c = USART1_Receive();
		if (c == endChar) break;
		str[i] = c;
		i++;
	}
	return i + 1;
}