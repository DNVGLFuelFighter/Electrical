/*
 * Front_lights_module2.c
 *
 * Created: 3/24/2016 4:04:57 PM
 * Author : Vilius
 */ 

#include <avr/interrupt.h>
#include "definitions.h"
#include "can.h"
#include "USART.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "front_lights_module.h"

void inits(void) {
	can_init();
	USART_init(MYUBRR, true);
	front_lights_init();
	
	printf("\r\nInitialization complete");
}
void LED_on() {
	toggle_bit(DDRB, PB7);
	_delay_ms(500);
	printf("Hey");
}

int main(void)
{
	BOOL ret;
	BOOL ret2;
	_delay_ms(500);
	inits();
	
	ret = prepare_rx(14, ID_lightShow, 0xff, light_show);
	ret2 = prepare_rx(0, ID_lightsFront, 0xff, light_handler);
	sei();

	for(;;)
		asm("sleep");;
	return 0;
}


