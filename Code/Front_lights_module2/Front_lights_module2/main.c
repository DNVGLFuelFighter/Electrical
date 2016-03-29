/*
 * Front_lights_module2.c
 *
 * Created: 3/24/2016 4:04:57 PM
 * Author : Vilius
 */ 

/* TODO: Implement handshake in can messages */

#include <avr/interrupt.h>
#include "definitions.h"
#include "can.h"
#include "USART.h"
#include <avr/io.h>
#include <stdio.h>
#include "front_lights_module.h"
#include "timer1.h"

void inits(void) {
	can_init();
	USART_init(MYUBRR, true);
	front_lights_init();
	timer1_init();
	
	printf("\r\nInitialization complete");
}

int main(void)
{
	BOOL ret;
	
	inits();
	
	ret = prepare_rx(0, ID_lightsFront, 0xff, front_light_handler);
	
	/* Set global interrupts */
	sei();
	
	for(;;)
		asm("sleep");;
	return 0;
}

