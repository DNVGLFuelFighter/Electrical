/*
 * Motor control 
 *
 * Created 03/04/2016
 * Author: Mark Hocevar
 * DNV GL Fuel Fighter 2016
 **
 *
 */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <util/delay.h>

#include "can.h"
#include "USART.h"





/* Init of motor control module  */
void init(void);
void can_recv(CAN_packet *p, unsigned char mob);

void can_send(void);
/* Timer for sending can message every x seconds */




