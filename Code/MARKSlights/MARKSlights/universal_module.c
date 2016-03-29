/***************************************************
 * 	Author: Mark Hocevar, DNVGL Fuel Fighter
 * 	Date: 	23.3.2016
 *
 * 	Source file used in all universal modules.
 * 	It includes basic data and configurations. 
 *
 *
 *
 ***************************************************/
#include "universal_module.h"

/********************************************************************
 * Control of onboard RGB led o
 *******************************************************************/
/**************
 * Init RGB led connected to PB5, PB6 and PB7
 ************ */
void RGB_init(){
	DDRB |= (1 << DDB5) | (1 << DDB6) | (1 << DDB7);
	PORTB |= (1 << PB5) | (1 << PB6) | (1 << PB7);
}

/*************
 * Turn on led.
 * 	@arg: pin number (only accepts pb5, pb6 or pb7)
 *************/
void RGB_led_off(int num){
	if(num == PB5 || num == PB6 || num == PB7){
		PORTB |= (1 << num); 
	}	
}

/*************
 * Turn off led.
 * 	@arg: pin number (only accepts pb5, pb6 or pb7)
 *************/
void RGB_led_on(int num){
	if(num == PB5 || num == PB6 || num == PB7){
		PORTB &= ~(1 << num); 
	}
}
/*************
 * Toggle led.
 * 	@arg: pin number (only accepts pb5, pb6 or pb7)
 *************/
void RGB_toggle(int num){
	if(num == PB5 || num == PB6 || num == PB7){
		PORTB ^= (1 << num); 
	}
}
	
