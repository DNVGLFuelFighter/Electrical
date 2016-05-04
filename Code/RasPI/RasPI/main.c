/*
 * RasPI.c
 *
 * Created: 4/15/2016 12:20:01 PM
 * Author : Vilius
 */ 

#include <avr/io.h>
#include "definitions.h"
#include <util/delay.h>
#include "USART.h"
#include "spi.h"
#include "can.h"

void inits( void);
unsigned char spi_send_can_packet(CAN_packet* p);

int main(void)
{
	set_bit(DDRB, DDB4);
	set_bit(PORTB, PB4);
	inits();
	CAN_packet p;
	p.id = 5;
	p.length = 8;
	p.data[0] = 111;
	p.data[1] = 222;
	p.data[2] = 33;
	p.data[3] = 44;
	p.data[4] = 55;
	p.data[5] = 66;
	p.data[6] = 77;
	p.data[7] = 88;

	unsigned char ret;
	
    for(;;) {
		_delay_ms(100);
		
// 		ret = spi_slave_rx(ACK);
// 		printf("\n\r Data - ");
// 		printf("%d",ret);
		
		ret = 0;
		ret = spi_send_can_packet(&p);
		if(ret == 10) {
			printf("\r\n The packet has been transmitted successfully");
			printf(" code = %u", ret);	
		}
		else {
			printf("\r\n An error has occurred in transmission");
			printf(" code = %u", ret);	
		}
		asm("sleep");
	}
}

void inits( void){
	USART_init(MYUBRR, TRUE);
	//can_init();
	//spi_init_slave();
	spi_init_master();
	//spi_master_tx(1);
	
	printf("\r\nInitialization done");
	set_bit(DDRB, PB6);
	sei();
}

unsigned char spi_send_can_packet(CAN_packet* p) {
	int start = 0;
	int finish = p->length;
	int counter = 0;
	unsigned char result;
	
	spi_master_tx(123);
	result = spi_master_tx(p->id);
	if(result==123)
		counter += 1;
	spi_master_tx(1);
	result = spi_master_tx(p->length);
	if(result==1)
		counter++;
	spi_master_tx(1);
	while(start != finish){
		result = spi_master_tx(p->data[start]);
		if(result==1)
			counter++;
		spi_master_tx(1);
		start++;
	}
	return counter;
}