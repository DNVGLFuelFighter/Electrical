/*
 * front_module.h
 *
 * Created: 4/8/2016 5:55:25 PM
 *  Author: Vilius
 */ 


#ifndef FRONT_MODULE_H_
#define FRONT_MODULE_H_

#include "definitions.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "spi.h"
#include "can.h"
#include "timer0.h"


extern BOOL HORN_ON;
extern BOOL WIPERS_ON;
extern BOOL FANS_ON;

#define OUT_E	0b00100110
#define OUT_F	0b00101110
#define OUT_G	0b00110110

void fm_msg_handler(CAN_packet* p, unsigned char mob);
void fm_init( void);
void fm_horn_handler( void);
void fm_wipers_handler( void);
void fm_fans_handler( void);
void fm_brake_watcher(CAN_packet *p, CAN_packet *r);
void fm_horn( float voltage);
void fm_wipers( float voltage);
int fm_voltage_to_DAC( float voltage);

#endif /* FRONT_MODULE_H_ */