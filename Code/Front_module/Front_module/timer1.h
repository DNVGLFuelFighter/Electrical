/*
 * timer1.h
 *
 * Created: 4/8/2016 3:08:22 PM
 *  Author: Vilius
 */ 


#ifndef TIMER1_H_
#define TIMER1_H_

#include "front_module.h"

#include <avr/interrupt.h>
#include <util/delay.h>

extern int	DIR_CNT;

void timer1_init( void);

#endif /* TIMER1_H_ */