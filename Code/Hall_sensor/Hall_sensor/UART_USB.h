/*********************************************************************************************************************

	Interrupt driven buffered UART                  
                                                                                                   
	Author:		Anders Guldahl			 12/2009					
	Edited by:  Jostein Furseth			 30.04.2013                 

**********************************************************************************************************************/


#ifndef UART_USB_H_
#define UART_USB_H_


#define UART_NO_DATA 0x0100
#define UART_BAUD_USB 9600

// #define BAUDREG_USB ((unsigned int)((F_CPU * 10) / (16UL * UART_BAUD_USB) - 5) / 10)
#define BAUDREG_USB (F_CPU/16/UART_BAUD_USB)-1

// size must be in range 2 .. 256
#define RX0_SIZE	250		// usable: RX0_SIZE + 2 (4 .. 258)
#define TX0_SIZE	250		// usable: TX0_SIZE + 1 (3 .. 257)


/*********************************************************************************************************************
	Initializes UART0 used for USB-bridge and empties TX-buffer
**********************************************************************************************************************/
void init_USB_USART(void);


/*********************************************************************************************************************
	Checks if rx_in buffer has been modified by interrupt, 0 = RX-buffer empty
**********************************************************************************************************************/
unsigned char rx_inModified_USB( void );


/*********************************************************************************************************************
	Returns 1 if TX-buffer is ready to receive data, 0 otherwise 
**********************************************************************************************************************/
unsigned char utx_ready_USB( void );


/*********************************************************************************************************************
	Send char direct on UART0
**********************************************************************************************************************/
int uputChar_USB(unsigned char c);


/*********************************************************************************************************************
	 Load char into TX-buffer
**********************************************************************************************************************/
int uloadChar_USB(unsigned char c);


/*********************************************************************************************************************
	Get next char from RX-buffer. Returns UART_NO_DATA if buffer is unchanged. Uses rx_inModified() 
**********************************************************************************************************************/
uint16_t ugetChar_USB( void );


/*********************************************************************************************************************
	Send string direct on UART0. Uses uputchar()
**********************************************************************************************************************/
void uputs_USB( unsigned char *s );


/*********************************************************************************************************************
	Fill buffer up to size, returns the number of char's written to buffer. Uses function ugetChar0()
**********************************************************************************************************************/
int ugets_USB(unsigned char *buffer, int size);


/*********************************************************************************************************************
	 Flush RX-buffer
**********************************************************************************************************************/
void rx_flush_USB( void );


#endif /* UART_USB_H_ */