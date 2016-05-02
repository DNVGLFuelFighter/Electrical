/*
 * Steering_wheel.c
 *
 * Created: 3/29/2016 3:58:34 PM
 * Author : Vilius
 */ 
#include "definitions.h"
#include "can.h"
#include "USART.h"
#include "steering_wheel.h"
#include "adc.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "timer1.h"
#include "timer0.h"

void inits( void) {
	can_init();
	USART_init(MYUBRR, true);
	sw_init();
	adc_init();
	timer1_init();
	timer0_init();
	printf("\r\nSteering wheel initialized");
	sei();
	/* I'm alive LED */
	set_bit(DDRB, PB6);
}

int main(void)
{
	/* Initialize module */
	inits();

    for(;;) {
		asm("sleep");;
	}
}
