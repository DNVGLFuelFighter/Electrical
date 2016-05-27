/*
 * USART.h
 *
 * Created: 02.07.2013 17:52:37
 *  Author: Ole
 */ 


#ifndef USART_H_
#define USART_H_

//USART0 is connected to the USB

void USART0_Init (unsigned int baud);
void USART1_Init (unsigned int baud);
void uart_putc0 (unsigned char data);
void uart_putc1 (unsigned char data);
void uart_puts0(const char *s );
void uart_puts1(const char *s );
unsigned char USART0_Receive (void);
unsigned char USART1_Receive (void);
unsigned char uart_receive0(char *str, unsigned char size, unsigned char endChar);
unsigned char uart_receive1(char *str, unsigned char size, unsigned char endChar);

#endif /* USART_H_ */