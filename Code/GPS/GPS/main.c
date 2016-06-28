/*
 * GPS.c
 *Created: 23.10.2014 15:54:46
 * Author: olestokk
 * Edited: 5/2/2016 6:26:35 PM
 * Author : Vilius
 */ 

#include <avr/io.h>
#include "definitions.h"
#include "ff.h"
#include "diskio.h"
#include "USART.h"
#include "can.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include "SDCard.h"

typedef enum {TIME, LAT, LAT_IND, LONGT, LONG_IND, POSFIX, NRSAT} gps_msg_index_t; //changed LONG to LONGT
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

BOOL FINISHED_LISTENING = TRUE;
BOOL FINISHED_WRITING_ESC = TRUE;
BOOL FINISHED_WRITING_POWER = TRUE;
BOOL FINISHED_WRITING_SW = TRUE;

void inits( void) {
	USART0_Init(MYUBRR0);
	USART1_Init(MYUBRR1);
	can_init();
	timer0_init();
	SDCard_init();
	//PORTF |= (1<<PF3); // GPS CS HIGH
	
	//LEDS on UM
	DDRB |= (1<<LED_RED) | (1<<LED_GREEN) | (1<<LED_BLUE);
	PORTB |= (1<<LED_BLUE) | (1<<LED_GREEN);
	PORTB &= ~(1<<LED_RED);
}

int main(void)
{	
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
	
	FATFS fs;
	
	uint8_t result;
	BOOL SDCard_closed = FALSE;
	
	/* Initialize module */
	inits();
	result = initializeFAT(&fs);
	
	if (result != 0)
		while(1)
			errorLED();
			
	printf("\r\nGPS initialized");
	
	//Open a text file on the SD Card
	if(f_open(&file_sw, "/SW.txt", FA_WRITE | FA_CREATE_ALWAYS) != FR_OK){
		while (1)
		errorLED();
	}
	if(f_open(&file_power, "/POWER.txt", FA_WRITE | FA_CREATE_ALWAYS) != FR_OK){
		while (1)
		errorLED();
	}
	if(f_open(&file_esc, "/ESC.txt", FA_WRITE | FA_CREATE_ALWAYS) != FR_OK){
		while (1)
		errorLED();
	}
	//Delay to ensure things are all set up and SD card is ready to receive more commands
	_delay_ms(500);
	
	prepare_rx(0, ID_steeringWheel, MASK_GPS, SDCard_callback);
	UCSR1B |= (1<<RXCIE1);
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
			
			if (gps_pos_fix[0] == '1') {
				printf("\r\ngps_pos_fix[0] = '1'");
				PORTB |= (1<<LED_RED);
				PORTB &= ~(1<<LED_GREEN);
			} else {
				printf("\r\ngps_pos_fix[0] unknown");
				PORTB |= (1<<LED_GREEN);
				PORTB &= ~(1<<LED_RED);
			}
			msg_done = false;
		} // if statement
		if(SDCard_closed){
			finishedLED();
		} else if (!test_bit(PINE, PE3)) {
			// turn off the timer0
			clear_bit(TIMSK0, TOIE0);
			TIMER0_SW_OVF = FALSE;
			FINISHED_LISTENING = TRUE;
			if (FINISHED_WRITING_SW && FINISHED_WRITING_POWER && FINISHED_WRITING_ESC && !SDCard_closed) {
				printf("\r\nClosing SD Card.");
				if (f_close(&file_sw) != FR_OK){
					while (1) {
						printf("\r\nCouldn't close SD Card.");
						_delay_ms(100);
						errorLED();
					}
				}
				if (f_close(&file_power) != FR_OK){
					while (1) {
						printf("\r\nCouldn't close SD Card.");
						_delay_ms(100);
						errorLED();
					}
				}
				if (f_close(&file_esc) != FR_OK){
					while (1) {
						printf("\r\nCouldn't close SD Card.");
						_delay_ms(100);
						errorLED();
					}
				}
				f_mount(0,0);
				printf("\r\nSD Card closed.");
				
				SDCard_closed = TRUE;
			}
		}
		asm("sleep");
    }
}

ISR(USART1_RX_vect) {
	unsigned char c = USART1_Receive();
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
					case LONGT:
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

