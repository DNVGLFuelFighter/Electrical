/*

 * New_front_module.c
 *
 * Created: 4/8/2016 5:50:54 PM
 * Author : Vilius
 */ 

#include "definitions.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "can.h"
#include "USART.h"
#include "front_module.h"

// Timers
#include "timer0.h"
#include "timer1.h"
#include "timer3.h"	

void inits( void);

int main(void)
{
	/* Initialize module */
    inits();
    for(;;) {
		fm_horn_handler();
		fm_wipers_handler();
		fm_fans_handler();
		_delay_ms(16); // the longer the wait the slower the wiper will go
		asm("sleep");
	}
	return 0;
}

void inits( void) {
	cli();
	USART_init(MYUBRR, TRUE);
	can_init();
	spi_init_master();
	fm_init();
	timer0_init();
	timer1_init();
	//TODO: RUN timer3_init();
	timer3_init();
	prepare_rx(1, ID_steeringWheel, MASK_FRONT_MODULE, fm_msg_handler);
	printf("\r\nFront module initialized");
	sei();
	set_bit(DDRB, DDB6);
	clear_bit(PORTB, PB6);
}