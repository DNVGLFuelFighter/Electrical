/*
 * steering_wheel.h
 *
 * Created: 3/29/2016 4:12:58 PM
 *  Author: Vilius
 */ 

#include "definitions.h"
#include "timer0.h"
#include "can.h"
#include "USART.h"

#ifndef STEERING_WHEEL_H_
#define STEERING_WHEEL_H_

void sw_init( CAN_packet*, CAN_packet*);
CAN_packet sw_input( void);

#endif /* STEERING_WHEEL_H_ */