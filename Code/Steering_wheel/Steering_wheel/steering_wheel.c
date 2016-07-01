/*
 * steering_wheel.c
 *
 * Created: 3/29/2016 4:12:29 PM
 *  Author: Vilius
 */ 

#include "steering_wheel.h"

BOOL sw_ind_right_change = FALSE;
BOOL sw_ind_left_change = FALSE;
BOOL sw_ind_right_temp = FALSE;
BOOL sw_ind_left_temp = FALSE;
BOOL sw_ind_logic = FALSE;
BOOL sw_cc_minus_temp = FALSE;
BOOL sw_cc_minus_change = FALSE;
BOOL sw_cc_plus_temp = FALSE;
BOOL sw_cc_plus_change = FALSE;

uint8_t cc_value = 0;

void sw_init( void) {	
	/* Indicators init */
	/* Set PE4 and PD0 as inputs */
	clear_bit(DDRD, DDD0);
	clear_bit(DDRE, DDE4);
	/* Set PE4 and PD0 as high (pull up res) */
	set_bit(PORTD, PD0);
	set_bit(PORTE, PE4);	
	
	/* Wipers init */
	clear_bit(DDRE, DDE5);
	set_bit(PORTE, PE5);
	
	/* Horn init */
	clear_bit(DDRB, DDB0);
	set_bit(PORTB, PB0);
	
	/* Deadman */
	clear_bit(DDRB, DDB4);
	set_bit(PORTB, PB4);
	
	/* Speed potentiometer uses adc_init() */
	
	/* Cruise control */
	clear_bit(DDRB, DDB2);
	clear_bit(DDRB, DDB1);
	set_bit(PORTB, PB2);
	set_bit(PORTB, PB1);	
}

void sw_input( CAN_packet* p) {	
	/* Initialize data */
	p->data[0] = 0;
	p->data[1] = 0;
	p->data[2] = 0;
	/* Read right/left indicator */
	if(!test_bit(PIND, PD0))
		p->data[0] |= (1<<0);
	else if(!test_bit(PINE, PE4))
		p->data[0] |= (1<<1);
// 	sw_ind_right_temp = !test_bit(PIND, PD0); 
// 	if((sw_ind_right_temp == TRUE && sw_ind_right_change == FALSE && sw_ind_left_change == FALSE)) {
// 		sw_ind_right_change = TRUE;
// 		/*sw_ind_logic = 1;*/
// 		p->data[0] |= (1<<0);
// 	} else if (sw_ind_right_temp == TRUE && sw_ind_right_change == TRUE /*&& sw_ind_logic == 1*/) {
// 		sw_ind_right_change = FALSE;
// 		/*sw_ind_logic = 0;*/
// 	}
// 	sw_ind_left_temp = !test_bit(PINE, PE4);
// 	if(sw_ind_left_temp == TRUE && sw_ind_left_change == FALSE && sw_ind_right_change == FALSE) {
// 		sw_ind_left_change = TRUE;
// 		p->data[0] |= (1<<1);
// 	} else {
// 		sw_ind_left_change = sw_ind_left_temp;
// 	}
	/* Read cruise control */
	sw_cc_minus_temp = !test_bit(PINB, PB1);
	sw_cc_plus_temp = !test_bit(PINB, PB2);
	if(sw_cc_minus_temp == TRUE && sw_cc_minus_change == FALSE){
		sw_cc_minus_change = TRUE;
		if(cc_value < 230)
		cc_value = cc_value+25;
	}else if(sw_cc_plus_temp == TRUE && sw_cc_plus_change == FALSE){
		sw_cc_plus_change = TRUE;
		if(cc_value > 25)
			cc_value = cc_value-25;
	}
	sw_cc_minus_change = sw_cc_minus_temp;
	sw_cc_plus_change = sw_cc_plus_temp;
	p->data[2] = cc_value;
	/* Read horn */
	if (!test_bit(PINB, PB0))
		p->data[0] |= (1<<4);
	/* Read wiper */
	if (!test_bit(PINE, PE5))
		p->data[0] |= (1<<5);
	/* Read CC deadman */
	if (!test_bit(PINB, PB4))
		p->data[0] |= (1<<6);
	else{
		cc_value = 0;
		p->data[2] = 0;
	}
}