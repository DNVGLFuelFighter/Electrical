/*
 * steering_wheel.h
 *
 * Created: 3/29/2016 4:12:58 PM
 *  Author: Vilius
 */ 



#ifndef STEERING_WHEEL_H_
#define STEERING_WHEEL_H_

#include "definitions.h"
#include "can.h"
#include "USART.h"
#include <avr/interrupt.h>

void sw_init( void);
void sw_input( CAN_packet*);

#endif /* STEERING_WHEEL_H_ */