/*
 * GPS.c
 *Created: 23.10.2014 15:54:46
 * Author: olestokk
 * Edited: 5/2/2016 6:26:35 PM
 * Author : Vilius
 */ 

#include <avr/io.h>
#include "definitions.h"
#include "USART.h"
#include "can.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {TIME, LAT, LAT_IND, LONG, LONG_IND, POSFIX, NRSAT} gps_msg_index_t;
typedef enum {ID, GPGGA, MESSAGE} gps_index_t;
	
#define GPS_TIME_ID	21
#define GPS_LAT1_ID	22
#define GPS_LAT2_ID	23
#define GPS_LONG1_ID	24
#define GPS_LONG2_ID	25

//volatile means that the variables can be unexpectedly changed any time
//like if an interrupt occurs in the middle of a function testing the variable
char bufferIn[50];
unsigned char gps_time[8];
char gps_lat[10];
char gps_lat_ind[2];
char gps_long[10];
char gps_long_ind[2];
char gps_pos_fix[2];
char gps_nr_sat[3];

unsigned char counter = 0;
gps_msg_index_t gps_msg_index = TIME;
gps_index_t gps_index = ID;
volatile bool msg_done = false;

#define LED_BLUE PB7
#define LED_GREEN PB6
#define LED_RED	PB5

void inits( void) {
	can_init();
	USART_init(MYUBRR0, TRUE);
	
	//PORTF |= (1<<PF3); // GPS CS HIGH
	
	//LEDS on UM
	DDRB |= (1<<LED_RED) | (1<<LED_GREEN) | (1<<LED_BLUE);
	PORTB |= (1<<LED_BLUE) | (1<<LED_GREEN);
	PORTB &= ~(1<<LED_RED);
	
	printf("\n\rGPS module initialized");
}

int main(void)
{
	/* Initialize module */
	inits();
	
	CAN_packet can_time;
	can_time.id = GPS_TIME_ID;
	can_time.length = 8;
	
	CAN_packet can_lat1;
	can_lat1.id = GPS_LAT1_ID;
	can_lat1.length = 2;
	
	CAN_packet can_lat2;
	can_lat2.id = GPS_LAT2_ID;
	can_lat2.length = 8;
	
	CAN_packet can_long1;
	can_long1.id = GPS_LONG1_ID;
	can_long1.length = 3;
	
	CAN_packet can_long2;
	can_long2.id = GPS_LONG2_ID;
	can_long2.length = 8;
	
	sei();
	
    for(;;) {
		if (msg_done) {	
			for (unsigned char i = 0; i < can_time.length; i++) {
				can_time.data[i] = gps_time[i];
				//uart_putc0(can_time.data[i]);
			}
			can_tx(14, &can_time);
			//uart_putc0('\n');
			
			for (unsigned char i = 0; i < can_lat1.length; i++) {
				can_lat1.data[i] = gps_lat[i];
				//uart_putc0(can_lat1.data[i]);
			}
			can_tx(13, &can_lat1);
			//uart_putc0(' ');
			
			for (unsigned char i = 0; i < can_lat2.length-1; i++) {
				can_lat2.data[i] = gps_lat[i+can_lat1.length];
				//uart_putc0(can_lat2.data[i]);
			}
			can_lat2.data[can_lat2.length-1] = gps_lat_ind[0];
			//uart_putc0(can_lat2.data[can_lat2.length-1]);
			can_tx(12, &can_lat2);
			//uart_putc0(' ');
			
			for (unsigned char i = 0; i < can_long1.length; i++) {
				can_long1.data[i] = gps_long[i];
				//uart_putc0(can_long1.data[i]);
			}
			can_tx(11, &can_long1);
			//uart_putc0(' ');
			
			for (unsigned char i = 0; i < can_long2.length-1; i++) {
				can_long2.data[i] = gps_long[i+can_long1.length];
				//uart_putc0(can_long2.data[i]);
			}
			can_long2.data[can_long2.length-1] = gps_long_ind[0];
			//uart_putc0(can_long2.data[can_long2.length-1]);
			can_tx(10, &can_long2);
			//uart_putc0('\n');
			
			if (gps_pos_fix[0] = '1') {
				printf("\r\ngps_pos_fix[0] = '1'");
				PORTB |= (1<<LED_RED);
				PORTB &= ~(1<<LED_GREEN);
			} else {
				printf("\r\ngps_pos_fix[0] unknown");
				PORTB |= (1<<LED_GREEN);
				PORTB &= ~(1<<LED_RED);
			}
			
			/*
			uart_puts0("Time: ");
			uart_puts0(gps_time);
			uart_putc0('\n');
			
			uart_puts0("Latitude: ");
			uart_puts0(gps_lat);
			uart_putc0('\n');
			
			uart_puts0("Latitude indicator: ");
			uart_puts0(gps_lat_ind);
			uart_putc0('\n');
			
			uart_puts0("Longitude: ");
			uart_puts0(gps_long);
			uart_putc0('\n');
			
			uart_puts0("Longitude indicator: ");
			uart_puts0(gps_long_ind);
			uart_putc0('\n');
			
			uart_puts0("Position fix: ");
			uart_puts0(gps_pos_fix);
			uart_putc0('\n');
			
			uart_puts0("Nr of satelites: ");
			uart_puts0(gps_nr_sat);
			uart_putc0('\n');
			*/
			
			//uart_putc0('\n');
			msg_done = false;
		}
		asm("sleep");
    }
}

ISR(USART0_RX_vect) {
	unsigned char c = USART_rx();
	printf("\r\n%c", c);
	switch(gps_index){
		case ID:
			if(c == '$')
			{
				gps_index = GPGGA;
				printf("\r\ngps_index = GPGGA");
			}
			break;
		case GPGGA:
			if (c == ',')
			{
				if (strcmp(bufferIn, "GPGGA") == 0)
				{
					gps_index = MESSAGE;
					msg_done = false;
					printf("\r\ngps_index = MESSAGE");
				}
				else
				{
					gps_index = ID;
					printf("\r\ngps_index = ID");
				}
				memset(bufferIn, 0, sizeof bufferIn);
				counter = 0;
			} 
			else
			{
				bufferIn[counter] = c;
				counter++;
			}
			break;
		case MESSAGE:
			if (c == ',')
			{
				//uart_puts0(bufferIn);
				//uart_putc0('\n');
				switch(gps_msg_index){
					case TIME:
						strcpy(gps_time, bufferIn);
						break;
					case LAT:
						strcpy(gps_lat, bufferIn);
						break;
					case LAT_IND:
						strcpy(gps_lat_ind, bufferIn);
						break;
					case LONG:
						strcpy(gps_long, bufferIn);
						break;
					case LONG_IND:
						strcpy(gps_long_ind, bufferIn);
						break;
					case POSFIX:
						strcpy(gps_pos_fix, bufferIn);
						break;
					case NRSAT:
						strcpy(gps_nr_sat, bufferIn);
						msg_done = true;
						break;
				}
				memset(bufferIn, 0, sizeof bufferIn);
				counter = 0;
				gps_msg_index++;
				if (msg_done)
				{
					gps_index = ID;
					gps_msg_index = TIME;
				}
			}
			else
			{
				bufferIn[counter] = c;
				counter++;
			}
			break;
	}	
}

