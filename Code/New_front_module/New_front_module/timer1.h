/*
 * timer1.h
 *
 * Created: 3/25/2016 1:47:51 PM
 *  Author: Vilius
 */ 

/* 16-bit timer */
/* Use Clear Timer on Compare Match (CTC) Mode? p.127 */

#ifndef TIMER1_H_
#define TIMER1_H_

#include <avr/interrupt.h>
#include "can.h"

#define BOTTOM	0x0000
#define MAX		0xFFFF
#define TOP		0x00FF //prone to change

void timer1_init( void);
unsigned int TIM16_ReadTCNT1( void);
void TIM16_WriteTCNT1( unsigned int i);

#endif /* TIMER1_H_ */