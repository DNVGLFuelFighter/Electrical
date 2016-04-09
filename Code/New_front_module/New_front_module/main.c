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
//----------------------------------
// #include "timer0.h" // 8 bit timers
// #include "timer2.h"	
// // ---------------------------------
// #include "timer1.h" // 16 bit timers
#include "timer3.h"	//	
// ---------------------------------

void inits( void);

int main(void)
{
	/* Create two CAN packets */
	CAN_packet msg_old;
	CAN_packet msg_new;
	
	/* Initialize created CAN packets */
	msg_old.id = ID_brakes;
	msg_old.length = 1;
	msg_new.id = ID_brakes;
	msg_new.length = 1;
	
    inits();
    for(;;) {
		fm_horn_handler();
		fm_wipers_handler();
		fm_fans_handler();
		fm_brake_watcher(&msg_old, &msg_new);
		asm("sleep");;
	}
	return 0;
}

void inits( void) {
	USART_init(MYUBRR, TRUE);
	can_init();
	spi_init_master();
	fm_init();
	//TODO: RUN timer3_init();
	prepare_rx(1, ID_steeringWheel, MASK_FRONT_MODULE, fm_msg_handler);
	printf("\r\nInitialization done");
	sei();
}

