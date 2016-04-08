/*
 * timer2.c
 *
 * Created: 4/7/2016 1:33:04 PM
 *  Author: Vilius
 */ 

#include "timer2.h"

BOOL CW = TRUE;
BOOL CCW = FALSE;
int PERIOD_OVF_95 = 0;
int DIR_OVF_3125 = 0;

void timer2_init( void) {
	/* clk/1 */
	set_bit(TCCR2A, CS20);
	/* Enable timer overflow */
	set_bit(TIMSK2, TOIE2);
	sei();	
}

ISR(TIMER2_OVF_vect) {
	sei();
	if(WIPERS_ON) 
		fm_wiper(5);
		if(DIR_OVF_3125 >= 3125){
			if(CW) {
				CW = FALSE;
				CCW = TRUE;
			} else {
				CW = TRUE;
				CCW = FALSE;
			}
// 			DIR_OVF_3125 = 0;
// 		} else
// 			DIR_OVF_3125++;
// 	
// 		if(PERIOD_OVF_95 < 95)
// 			PERIOD_OVF_95++;
// 		/* Time to set high PB4 */
// 		else {
// 			set_bit(PORTB, PB4);
// 			if(CW) {
// 				_delay_us(1150);
// 			} else if(CCW) {
// 				_delay_us(1850);
// 			} else {
// 				_delay_us(1500);
// 			}
// 			PERIOD_OVF_95 = 0;
// 		}
// 	} else
// 		fm_wiper(0);
	}
	clear_bit(PORTB, PB4);
	
}

// int CW_OVF_36 = 0;
// int CCW_OVF_58 = 0;
// int NEUTRAL_OVF_47 = 0;
// int PERIOD_OVF_95 = 0;
// BOOL change_direction = FALSE;


// 	if(WIPERS_ON) {
// 		
// 		fm_wiper(5);
// 		if(COUNTER == 6) {
// 			if (change_dir) {
// 				set_bit(PORTB, PB4);
// 				_delay_us(1000);
// 		} else if ((COUNTER == 6) & (change_dir >= 389) & (change_dir < 779)){
// 			//printf("\r\nBack");
// 			set_bit(PORTB, PB4);
// 			_delay_us(1400);
// 		} else {
// 			COUNTER = COUNTER+1;
// 			
// 			if (change_dir >= 779)
// 				change_dir = 0;
// 		}
// 		change_dir = change_dir+1;
// 	} else {
// 		fm_wiper(0);
// 		COUNTER = 0;
// 		change_dir = 0;
// 	}	
// 	clear_bit(PORTB, PB4);
// 	sei();	
