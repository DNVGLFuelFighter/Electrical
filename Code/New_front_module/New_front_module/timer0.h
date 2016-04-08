/*
 * timer0.h
 *
 * Created: 3/25/2016 1:31:14 PM
 *  Author: Vilius
 */ 
/* 8-bit timer */


#ifndef TIMER0_H_
#define TIMER0_H_

#include "front_module.h"
#include <avr/interrupt.h>
#include <util/delay.h>

extern unsigned char OVF_CNT;

void timer0_init( void);



#endif /* TIMER0_H_ */