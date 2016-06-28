/*
 * SDCard.c
 *
 * Created: 7/27/2015
 *  Company: TekBots @ Oregon State University
 *	Description: This program is designed for the Teensy 2.0. This uses
 *			an SD card module (from TekBots) to write ADC values to an SD Card.
 *			The Teensy communicates through SPI to the SD card module. The LED
 *			on the Teensy gives the user visual feedback about what is happening
 *			within the program (descriptions follow).
 * 	LED Description: The LED on the Teensy turns on and off to signal
 *						different modes
 *						SOLID		---> Initializing
 *						SHORT-SHORT ---> Error occurred
 *						SHORT-LONG	---> Writing to SD Card
 *						LONG-LONG	---> Program Completed
 *
 *	It should be known that this code may not work when compiled with any optimization level
 *	other than -O1. This would be found in the Makefile.
 *  
 *  This code has been successfully tested with a 1GB SD card and a 32GB SDHC card.
 *	Edited on: 5/7/2016
 *	Edited by: Vilius Ciuzelis
 *	Company: DNV GL Fuel Fighter @ Norwegian University of Science and Technology (NTNU)
 */ 

#include "SDCard.h"

char buf_sw[5];
char buf_power[13]; // why not 17? --> Can't close with 17
char buf_esc[19];
UINT bytesWritten_sw;
UINT bytesWritten_power;
UINT bytesWritten_esc;

void SDCard_callback(CAN_packet *p, unsigned char mob) {
	(void)mob;
	switch(p->id) {
		case ID_steeringWheel:
			if(FINISHED_WRITING_SW && TIMER0_SW_OVF)
				SDCard_sw(p);
			break;
		case ID_power:
			if(FINISHED_WRITING_POWER && TIMER0_POWER_OVF)
				SDCard_power(p);
			break;
		case ID_esc_telemetry:
			if(FINISHED_WRITING_ESC && TIMER0_ESC_OVF)
				SDCard_esc(p);
			break;
	}
}

void SDCard_esc(CAN_packet *p) {
	if (FINISHED_LISTENING) {
		//do nothing
	} else if (FINISHED_WRITING_ESC && !FINISHED_LISTENING) {
		cli();
		FINISHED_WRITING_ESC = FALSE;
		// write stuff
		BLUELEDON;
		GREENLEDOFF;
		int numBytes = 20;
		uint16_t rpm;
		rpm = ((p->data[3])<<8);
		rpm |= (p->data[4]);
		sprintf(buf_esc,"%d %d %d %d\r\n", p->data[0], p->data[1], p->data[2], rpm);
		
		printf("\r\nWriting %s", buf_esc);
		if (f_write(&file_esc, &buf_esc, numBytes, &bytesWritten_esc) != FR_OK) {
			while(1) {
				printf("\r\nCouldnt write esc buffer");
				_delay_ms(50);
				errorLED();
			}
		}
		BLUELEDOFF;
		FINISHED_WRITING_ESC = TRUE;
		sei();
	}
}

void SDCard_power(CAN_packet *p) {
	cli();
	FINISHED_WRITING_POWER = FALSE;
	// write stuff
	BLUELEDON;
	GREENLEDOFF;
	int numBytes = 13;
	uint16_t voltage;
	voltage = ((p->data[0])<<8);
	voltage |= (p->data[1]);
	uint16_t current;
	current = ((p->data[2])<<8);
	current |= (p->data[3]);	
	printf("\r\nVoltage %d current %d", voltage, current);
	sprintf(buf_power,"%d %d\r\n", current, voltage);

	if (f_write(&file_power, &buf_power, numBytes, &bytesWritten_power) != FR_OK) {
		while(1) {
			printf("\r\nCouldnt write power buffer");
			_delay_ms(50);
			errorLED();
		}
	}
	BLUELEDOFF;
	FINISHED_WRITING_POWER = TRUE;
	sei();
}

void SDCard_sw(CAN_packet *p) {
	if(FINISHED_WRITING_SW) {
		cli();
		FINISHED_WRITING_SW = FALSE;
		// write stuff
		BLUELEDON;
		GREENLEDOFF;
		int numBytes = 5;
		
		sprintf(buf_sw,"%d\r\n", p->data[1]);
		
		if (f_write(&file_sw, &buf_sw, numBytes, &bytesWritten_sw) != FR_OK) {
			while(1) {
				printf("\r\nCouldnt write sw buffer");
				printf("\r\nTIMER0_SW_OVF %d", TIMER0_SW_OVF);
				_delay_ms(50);
				errorLED();
			}
		}
		BLUELEDOFF;
		TIMER0_SW_OVF = FALSE;
		FINISHED_WRITING_SW = TRUE;
		sei();
	}
}

void SDCard_init(){
	//0 --> Input
	//1 --> Output
	DDRB = 0b11100111;
	PORTB |= (1<<5)|(1<<6)|(1<<7);
	DDRD = 0b00000000;
	DDRF = 0b00000000;
	DDRE = 0b00000000;
	set_bit(PORTE, PE3);
	
	//Set Up the SPI in Master Mode
	DDRB |= (1<<2)|(1<<1)|(1<<0);
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	
	USART0_Init(MYUBRR0);
	USART1_Init(MYUBRR1);
	can_init();
	sei();
}

uint8_t initializeFAT(FATFS* fs){
	DSTATUS driveStatus;
	
	// Mount and verify disk type
	if (f_mount(0, fs) != FR_OK){
		// Report error
		return 2;
	}
	
	driveStatus = disk_initialize(0);
	
	// Verify that disk exists
	if (driveStatus & STA_NODISK){
		// Report error
		return 2;
	}
	
	// Verify that disk is initialized
	if (driveStatus & STA_NOINIT){
		// Report error
		return 2;
	}
	
	// Verify that disk is not write protected
	if (driveStatus & STA_PROTECT){
		// Report error
		return 2;
	}
	
	return 0;
}
	
//Turns on the error signal
void errorLED( void){
	REDLEDON;
	_delay_ms(75);
	REDLEDOFF;
	_delay_ms(75);
}

void finishedLED( void){
	GREENLEDON;
	_delay_ms(400);
	GREENLEDOFF;
	_delay_ms(400);
}