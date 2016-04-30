/*
 * Dashboard_module.c
 *
 * Created: 4/5/2016 11:07:41 AM
 * Author : Vilius
 */ 

#include <avr/io.h>
#include "definitions.h"
#include <util/delay.h>
#include "dashboard_module.h"
#include "USART.h"
#include "can.h"
#include "timer0.h"
#include "timer1.h"

void inits();

int main(void)
{	
	/* Initialize module*/
	inits();
	
    for(;;) 
		asm("sleep");
	return 0;
}

void inits( void) {
	USART_init(MYUBRR, TRUE);
	can_init();
	db_init();
	timer0_init();
	timer1_init();
	printf("\r\nDashboard initialized");
	sei();
	set_bit(DDRB, PB6);
}

