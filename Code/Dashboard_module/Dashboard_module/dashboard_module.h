/*
 * dashboard_module.h
 *
 * Created: 4/5/2016 11:19:25 AM
 *  Author: Vilius
 */ 


#ifndef DASHBOARD_MODULE_H_
#define DASHBOARD_MODULE_H_

#include "definitions.h"
#include "can.h"
#include "USART.h"
#include <avr/interrupt.h>

void db_init( void);
void db_input( CAN_packet*);



#endif /* DASHBOARD_MODULE_H_ */