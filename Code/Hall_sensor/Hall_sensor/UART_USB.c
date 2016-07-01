
#define F_CPU 8.0E6

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "UART_USB.h"


static volatile unsigned char rx_USB_buff[RX0_SIZE] = {0};
static volatile unsigned char rx_USB_in;
static volatile unsigned char rx_USB_out;

static volatile unsigned char tx_USB_buff[TX0_SIZE] = {0};
static volatile unsigned char tx_USB_in;
static volatile unsigned char tx_USB_out;


#ifndef ROLLOVER
// count up and wrap around
#define ROLLOVER( x, max )	x = ++x >= max ? 0 : x

#endif


void init_USB_USART(){
	unsigned int brr;

	brr = BAUDREG_USB;

	// Set baud rate
	UBRR0H = (unsigned char)(brr>>8);
	UBRR0L = (unsigned char)brr;
	
	// Enable receiver and transmitter and rx-interrupt 
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);

	// Set frame format: 8data, 1stop bit 
	UCSR0C = (3<<UCSZ00);

	rx_USB_in  = 0;				
  	rx_USB_out = 0;
  	tx_USB_in  = 0;
  	tx_USB_out = 0;
}


unsigned char rx_inModified_USB( void )
{
	return rx_USB_out ^ rx_USB_in;		// rx_USB_in modified by interrupt !
}


unsigned char utx_ready_USB( void )
{
	unsigned char i = tx_USB_in;

	ROLLOVER( i, TX0_SIZE );
	return tx_USB_out ^ i;		// 0 = busy
}


int uputChar_USB( unsigned char c )
{
	UDR0 = c;	// Send byte
	
	while ( !( UCSR0A & (1<<UDRE0)) ); // Wait for empty transmit buffer

	return 0;
}


int uloadChar_USB(unsigned char c)
{
	unsigned char i = tx_USB_in;
	
	ROLLOVER( i, TX0_SIZE );
	
	if( i == tx_USB_out )		// buffer overflow
	{
		return -1;
	}
	UCSR0B |= (1<<UDRIE0);		// enable TX interrupt
		
	tx_USB_buff[tx_USB_in] = c;	
	tx_USB_in = i;	
}


uint16_t ugetChar_USB( void )
{
	unsigned char data;

	if(!rx_inModified_USB())			// return uart_no_data if no new char arrived
	{ 
  		return UART_NO_DATA;
	}

	data = rx_USB_buff[rx_USB_out];		// get byte
	
	ROLLOVER( rx_USB_out, RX0_SIZE );
	UCSR0B |= (1<<RXCIE0);				// enable RX interrupt
	
	return data;
}


void uputs_USB( unsigned char *s )
{
	while( *s )
    uputChar_USB( *s++ );
}


int ugets_USB(unsigned char *buffer, int size){

	int i;

	for(i = 0;i<size;i++)
	{
		if(rx_inModified_USB() != 0)
		{
			buffer[i] = ugetChar_USB();
		}
		else
		{
			return i;
		}
		
	}
	return i;
}


void rx_flush_USB( void )
{
	while(ugetChar_USB() != UART_NO_DATA);
}


//Interrupt driven USART receive. Checks if buffer contains complete messages
ISR(USART0_RX_vect )
{
	unsigned char i = rx_USB_in;

	ROLLOVER( i, TX0_SIZE );
	
	if( i == rx_USB_out )			// buffer overflow
	{			
		UCSR0B &= ~(1<<RXCIE0);		// disable RX interrupt
		return;
	}
	
	rx_USB_buff[rx_USB_in] = UDR0;
	rx_USB_in = i;
}


// Interrupt driven USART send. Sends data from TX-buffer when ready.
ISR(USART0_UDRE_vect)
{
	if( tx_USB_in == tx_USB_out )	// nothing to sent
	{		
		UCSR0B &= ~(1<<UDRIE0);		// disable TX interrupt
		return;
	}
	
	UDR0 = tx_USB_buff[tx_USB_out];
	
	ROLLOVER(tx_USB_out, TX0_SIZE );
}