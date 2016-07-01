/*
 * Front_lights_module2.c
 *
 * Created: 3/24/2016 4:04:57 PM
 * Author : Vilius
 */ 

/* TODO: Implement handshake in can messages */

#include "definitions.h"
#include <avr/interrupt.h>
#include "can.h"
#include "USART.h"
#include <avr/io.h>
#include <stdio.h>
#include "front_lights_module.h"
#include "timer1.h"
#include "timer0.h"

void inits(void) {
	can_init();
	USART_init(MYUBRR, true);
	front_lights_init();
	timer1_init();
	//timer0_init();
	
	printf("\r\nFront lights initialized\n");
	set_bit(DDRB, DDB6);
	clear_bit(PORTB, PB6);
}

int main(void)
{
	cli();
	inits();
	prepare_rx(0, ID_steeringWheel, MASK_FRONT_LIGHTS, front_light_handler);
	/* Set global interrupts */
	sei();
	
	for(;;) {
		asm("sleep");;
	}
	return 0;
}



