/*
 * timer0.h
 *
 * Created: 3/25/2016 1:31:14 PM
 *  Author: Vilius
 */ 
/* 8-bit timer */

#ifndef TIMER0_H_
#define TIMER0_H_

#include <avr/interrupt.h>
#include "definitions.h"

extern BOOL TIMER0_SW_OVF;
extern BOOL TIMER0_POWER_OVF;
extern BOOL TIMER0_ESC_OVF;

void timer0_init( void);



#endif /* TIMER0_H_ */