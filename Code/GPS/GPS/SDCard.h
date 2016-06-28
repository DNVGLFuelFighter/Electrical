/*
 * SDCard.h
 *
 * Created: 5/11/2016 8:16:15 PM
 *  Author: Vilius
 */ 


#ifndef SDCARD_H_
#define SDCARD_H_

#include <avr/io.h>
#include <stdio.h>
#include "ff.h"
#include "diskio.h"
#include "definitions.h"
#include <util/delay.h>
#include "USART.h"
#include "can.h"
#include "timer0.h"

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

FIL file_sw;
FIL file_power;
FIL file_esc;
extern BOOL FINISHED_LISTENING;
extern BOOL FINISHED_WRITING_SW;
extern BOOL FINISHED_WRITING_POWER;
extern BOOL FINISHED_WRITING_ESC;

void SDCard_init( void);
uint8_t initializeFAT(FATFS* fs);
void errorLED( void);
void finishedLED( void);
void SDCard_esc(CAN_packet *p);
void SDCard_power(CAN_packet *p);
void SDCard_sw(CAN_packet *p);
void SDCard_callback(CAN_packet *p, unsigned char mob);


#endif /* SDCARD_H_ */