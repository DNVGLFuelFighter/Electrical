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
 */ 

/*#define F_CPU 2000000UL*/

#include <avr/io.h>

#include <stdio.h>
#include "ff.h"
#include "diskio.h"
#include "definitions.h"
#include <util/delay.h>
#include "USART.h"


#define BLUELEDOFF PORTB |= (1<<7)
#define BLUELEDON PORTB &= ~(1<<7)
#define GREENLEDOFF PORTB |= (1<<6)
#define GREENLEDON PORTB &= ~(1<<6)
#define REDLEDON PORTB &= ~(1<<5)
#define REDLEDOFF PORTB |= (1<<5)

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
#define CPU_16MHz       0x00
#define CPU_8MHz        0x01
#define CPU_4MHz        0x02 
#define CPU_2MHz        0x03
#define CPU_1MHz        0x04
#define CPU_500kHz      0x05
#define CPU_250kHz      0x06
#define CPU_125kHz      0x07
#define CPU_62kHz       0x08

/**********
** This function initializes the Port Pin Directions of the
** used pins. This function also sets up the SPI in master mode.
** The ADC is also set up. It's using ADC0, in Free Running mode,
** and the clock rate has been divided so it's in the more acceptable
** frequency range (50kHz - 200kHz) for the ADC.
**********/
void Initialize(){
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
		
	//Set up the ADC (Using ADC0)
	ADMUX |= (1<<REFS0)|(1<<ADLAR);	//Sets reference to AVCC and left aligns ADC value
	ADCSRA |= (1<<ADEN)|(1<<ADSC)|(1<<ADATE)|(1<<ADPS1);		//Enables ADC, Start ADC, Auto Trigger, and Divides F_CPU by 16
	ADCSRB &= ~(1<<ADTS0)|~(1<<ADTS1)|~(1<<ADTS2)/*|~(1<<ADTS3)*/;		//Free running mode
	USART_init(MYUBRR1, TRUE);
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

//Turns on the error signal on the Teensy
void errorLED(void){
	REDLEDON;
	_delay_ms(75);
	REDLEDOFF;
	_delay_ms(75);	
}

int main(void){
	FATFS fs;
	FIL log;
	uint8_t result;
	UINT bytesWritten;
	uint8_t adcValue;
	uint16_t i;
	char buf[5];
		
	//CPU_PRESCALE(CPU_8MHz);
	Initialize();
	GREENLEDON;
	

	//Initialize the SD Card
	result = initializeFAT(&fs);
	if (result != 0){
		while(1)
			errorLED();
	}
	
	//Open a text file on the SD Card
	if(f_open(&log, "/data.txt", FA_WRITE | FA_CREATE_ALWAYS) != FR_OK){
		while (1)
			errorLED();
	}
	
	//Delay to ensure things are all set up and SD card is ready to receive more commands
	_delay_ms(500);
	
	//Loops through taking ADC measurements for a certain number of times
	//This could easily be changed to triggering on a button press or other 
	//		method of initiating
	int numBytes = 3;
	while(1) {
		
		BLUELEDON;
		
		//We only have 8 bits of precision and the values are left aligned,
		//	so we only look at the high (ADCH) 8 bits of the ADC value (10 bits total)
		adcValue = ADCH;
		
		//sprintf puts our values in decimal into a string (easier to 'print')
		sprintf(buf,"%d\r\n",adcValue);
		
		//This writes our array with the ADC value to the SD card
		if(adcValue < 10)
			numBytes = 3;
		else if(adcValue < 100)
			numBytes = 4;
		else
			numBytes = 5;
		if (f_write(&log, buf, numBytes, &bytesWritten) != FR_OK){
			while(1)
				errorLED();
		}
		
		BLUELEDOFF;
		//The ADC value updates slowly (comparatively)
		_delay_ms(50);
		if(!test_bit(PINE, PE3))
			break;
	}
	
	//Close the SD card
	if (f_close(&log) != FR_OK){
		while (1)
			errorLED();
	}
	
	f_mount(0,0);
	printf("\r\nDone!");
    while(1){
		GREENLEDON;
		_delay_ms(400);
		GREENLEDOFF;
		_delay_ms(400);
		
    }//While Loop
}//Main*/