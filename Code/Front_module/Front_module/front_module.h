/*
 * front_module.h
 *
 * Created: 3/31/2016 10:11:25 AM
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
#include "timer1.h"


#define OUT_E	0b00100110
#define OUT_F	0b00101110
#define OUT_G	0b00110110

extern BOOL HORN_ON;
extern BOOL WIPERS_ON;
extern BOOL FANS_ON;

void fm_init( void);
void fm_horn_init( void);
void fm_wiper_init( void);
void fm_fans_init( void);
void fm_horn(float voltage);
void fm_wiper(float voltage);
void fm_fans(float voltage);
int fm_voltage_to_DAC(float voltage);
void fm_brake( CAN_packet* p);
void fm_handler(CAN_packet* p, unsigned char mob);
void fm_wiper_routine( void);

#endif /* FRONT_MODULE_H_ */