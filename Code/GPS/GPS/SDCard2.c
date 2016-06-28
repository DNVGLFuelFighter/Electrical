// /*
//  * SDCard.c
//  *
//  * Created: 7/27/2015
//  *  Company: TekBots @ Oregon State University
//  *	Description: This program is designed for the Teensy 2.0. This uses
//  *			an SD card module (from TekBots) to write ADC values to an SD Card.
//  *			The Teensy communicates through SPI to the SD card module. The LED
//  *			on the Teensy gives the user visual feedback about what is happening
//  *			within the program (descriptions follow).
//  * 	LED Description: The LED on the Teensy turns on and off to signal
//  *						different modes
//  *						SOLID		---> Initializing
//  *						SHORT-SHORT ---> Error occurred
//  *						SHORT-LONG	---> Writing to SD Card
//  *						LONG-LONG	---> Program Completed
//  *
//  *	It should be known that this code may not work when compiled with any optimization level
//  *	other than -O1. This would be found in the Makefile.
//  *  
//  *  This code has been successfully tested with a 1GB SD card and a 32GB SDHC card.
//  *	Edited on: 5/7/2016
//  *	Edited by: Vilius Ciuzelis
//  *	Company: DNV GL Fuel Fighter @ Norwegian University of Science and Technology (NTNU)
//  */ 
// 
// #include <avr/io.h>
// 
// #include <stdio.h>
// #include "ff.h"
// #include "diskio.h"
// #include "definitions.h"
// #include <util/delay.h>
// #include "USART.h"
// #include "can.h"
// 
// #define BLUELEDOFF PORTB |= (1<<7)
// #define BLUELEDON PORTB &= ~(1<<7)
// #define GREENLEDOFF PORTB |= (1<<6)
// #define GREENLEDON PORTB &= ~(1<<6)
// #define REDLEDON PORTB &= ~(1<<5)
// #define REDLEDOFF PORTB |= (1<<5)
// 
// #define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
// #define CPU_16MHz       0x00
// #define CPU_8MHz        0x01
// #define CPU_4MHz        0x02 
// #define CPU_2MHz        0x03
// #define CPU_1MHz        0x04
// #define CPU_500kHz      0x05
// #define CPU_250kHz      0x06
// #define CPU_125kHz      0x07
// #define CPU_62kHz       0x08
// 
// void Initialize();
// uint8_t initializeFAT(FATFS* fs);
// void GPS_callback(CAN_packet *p, unsigned char mob);
// void errorLED( void);
// void finishedLED( void);
// 
// FIL file_sw;
// FIL file_power;
// FIL file_esc;
// BOOL FINISHED_LISTENING = FALSE;
// BOOL FINISHED_WRITING_SW = TRUE;
// BOOL FINISHED_WRITING_POWER = TRUE;
// BOOL FINISHED_WRITING_ESC = TRUE;
// char buf_sw[5];
// char buf_power[13]; // why not 17? --> Can't close with 17
// char buf_esc[19];
// UINT bytesWritten_sw;
// UINT bytesWritten_power;
// UINT bytesWritten_esc;
// 
// int main(void){
// 	FATFS fs;
// 	
// 	uint8_t result;
// 	BOOL ret = FALSE;
// 	BOOL SDCard_closed = FALSE;
// 	
// 	//CPU_PRESCALE(CPU_8MHz);
// 	Initialize();
// 	
// 	result = initializeFAT(&fs);
// 	
// 	if (result != 0)
// 		while(1)
// 			errorLED();
// 	GREENLEDON;
// 	printf("\r\nGPS initialized");
// 	//Open a text file on the SD Card
// 	if(f_open(&file_sw, "/SW.txt", FA_WRITE | FA_CREATE_ALWAYS) != FR_OK){
// 		while (1)
// 			errorLED();
// 	}
// 	if(f_open(&file_power, "/POWER.txt", FA_WRITE | FA_CREATE_ALWAYS) != FR_OK){
// 		while (1)
// 			errorLED();
// 	}
// 	if(f_open(&file_esc, "/ESC.txt", FA_WRITE | FA_CREATE_ALWAYS) != FR_OK){
// 		while (1)
// 			errorLED();
// 	}
// 	//Delay to ensure things are all set up and SD card is ready to receive more commands
// 	_delay_ms(500);
// 	GREENLEDON;
// 	
// 	ret = prepare_rx(0, ID_steeringWheel, MASK_GPS, GPS_callback);
// 	for(;;) {
// 		if(SDCard_closed){
// 				finishedLED();
// 		} else if (!test_bit(PINE, PE3)) {
// 			FINISHED_LISTENING = TRUE;
// 			if (FINISHED_WRITING_SW && FINISHED_WRITING_POWER && FINISHED_WRITING_ESC && !SDCard_closed) {
// 				printf("\r\nClosing SD Card.");
// 				if (f_close(&file_sw) != FR_OK){
// 					while (1) {
// 						printf("\r\nCouldn't close SD Card.");
// 						_delay_ms(100);
// 						errorLED();
// 					}
// 				}
// 				if (f_close(&file_power) != FR_OK){
// 					while (1) {
// 						printf("\r\nCouldn't close SD Card.");
// 						_delay_ms(100);
// 						errorLED();
// 					}
// 				}
// 				if (f_close(&file_esc) != FR_OK){
// 					while (1) {
// 						printf("\r\nCouldn't close SD Card.");
// 						_delay_ms(100);
// 						errorLED();
// 					}
// 				}
// 				f_mount(0,0);
// 				printf("\r\nSD Card closed.");
// 				SDCard_closed = TRUE;
// 			}	
// 		}
// 		asm("sleep");
// 	}
// }
// 
// void SDCard_esc(CAN_packet *p) {
// 	if (FINISHED_LISTENING) {
// 		//do nothing
// 	} else if (FINISHED_WRITING_ESC && !FINISHED_LISTENING) {
// 		cli();
// 		FINISHED_WRITING_ESC = FALSE;
// 		// write stuff
// 		BLUELEDON;
// 		GREENLEDOFF;
// 		int numBytes = 20;
// 		uint16_t rpm;
// 		rpm = ((p->data[3])<<8);
// 		rpm |= (p->data[4]);
// 		sprintf(buf_esc,"%d %d %d %d\r\n", p->data[0], p->data[1], p->data[2], rpm);
// 		
// 		printf("\r\nWriting %s", buf_esc);
// 		if (f_write(&file_esc, &buf_esc, numBytes, &bytesWritten_esc) != FR_OK) {
// 			while(1) {
// 				printf("\r\nCouldnt write esc buffer");
// 				_delay_ms(50);
// 				errorLED();
// 			}
// 		}
// 		BLUELEDOFF;
// 		FINISHED_WRITING_ESC = TRUE;
// 		sei();
// 	}
// }
// 
// void SDCard_power(CAN_packet *p) {
// 	if (FINISHED_LISTENING) {
// 		//do nothing
// 	} else if (FINISHED_WRITING_POWER && !FINISHED_LISTENING) {
// 		cli();
// 		FINISHED_WRITING_POWER = FALSE;
// 		// write stuff
// 		BLUELEDON;
// 		GREENLEDOFF;
// 		int numBytes = 13;
// 		uint16_t voltage;
// 		voltage = ((p->data[0])<<8);
// 		voltage |= (p->data[1]);
// 		uint16_t current;
// 		current = ((p->data[2])<<8);
// 		current |= (p->data[3]);	
// 		printf("\r\nVoltage %d current %d", voltage, current);
// 		sprintf(buf_power,"%d %d\r\n", current, voltage);
// 
// 		if (f_write(&file_power, &buf_power, numBytes, &bytesWritten_power) != FR_OK) {
// 			while(1) {
// 				printf("\r\nCouldnt write power buffer");
// 				_delay_ms(50);
// 				errorLED();
// 			}
// 		}
// 		BLUELEDOFF;
// 		FINISHED_WRITING_POWER = TRUE;
// 		sei();
// 	}
// }
// 
// void SDCard_sw(CAN_packet *p) {
// 	if (FINISHED_LISTENING) {
// 		//do nothing
// 	} else if (FINISHED_WRITING_SW && !FINISHED_LISTENING) {
// 		cli();
// 		FINISHED_WRITING_SW = FALSE;
// 		// write stuff
// 		BLUELEDON;
// 		GREENLEDOFF;
// 		int numBytes = 5;
// 		
// 		sprintf(buf_sw,"%d\r\n", p->data[1]);
// 
// 		printf("\r\nWriting %s", buf_sw);
// 		if (f_write(&file_sw, &buf_sw, numBytes, &bytesWritten_sw) != FR_OK) {
// 			while(1) {
// 				printf("\r\nCouldnt write sw buffer");
// 				_delay_ms(50);
// 				errorLED();
// 			}
// 		}
// 		BLUELEDOFF;
// 		FINISHED_WRITING_SW = TRUE;
// 		sei();
// 	}
// }
// 
// void GPS_callback(CAN_packet *p, unsigned char mob) {
// 	(void)mob;
// 	switch(p->id) {
// 		case ID_steeringWheel:
// 			if(FINISHED_WRITING_SW)
// 				SDCard_sw(p);
// 			break;
// 		case ID_power:
// 			if(FINISHED_WRITING_POWER)
// 				SDCard_power(p);
// 			break;
// 		case ID_esc_telemetry:
// 			if(FINISHED_WRITING_ESC)
// 				SDCard_esc(p);
// 			break;
// 	}
// }
// 
// void Initialize(){
// 	//0 --> Input
// 	//1 --> Output
// 	DDRB = 0b11100111;
// 	PORTB |= (1<<5)|(1<<6)|(1<<7);
// 	DDRD = 0b00000000;
// 	DDRF = 0b00000000;
// 	DDRE = 0b00000000;
// 	set_bit(PORTE, PE3);
// 	
// 	//Set Up the SPI in Master Mode
// 	DDRB |= (1<<2)|(1<<1)|(1<<0);
// 	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR0);
// 	
// 	USART0_Init(MYUBRR0);
// 	USART1_Init(MYUBRR1);
// 	can_init();
// 	sei();
// }
// 
// uint8_t initializeFAT(FATFS* fs){
// 	DSTATUS driveStatus;
// 	
// 	// Mount and verify disk type
// 	if (f_mount(0, fs) != FR_OK){
// 		// Report error
// 		return 2;
// 	}
// 	
// 	driveStatus = disk_initialize(0);
// 	
// 	// Verify that disk exists
// 	if (driveStatus & STA_NODISK){
// 		// Report error
// 		return 2;
// 	}
// 	
// 	// Verify that disk is initialized
// 	if (driveStatus & STA_NOINIT){
// 		// Report error
// 		return 2;
// 	}
// 	
// 	// Verify that disk is not write protected
// 	if (driveStatus & STA_PROTECT){
// 		// Report error
// 		return 2;
// 	}
// 	
// 	return 0;
// }
// 	
// //Turns on the error signal
// void errorLED( void){
// 	REDLEDON;
// 	_delay_ms(75);
// 	REDLEDOFF;
// 	_delay_ms(75);
// }
// 
// void finishedLED( void){
// 	GREENLEDON;
// 	_delay_ms(400);
// 	GREENLEDOFF;
// 	_delay_ms(400);
// }