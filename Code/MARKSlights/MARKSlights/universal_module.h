/***************************************************
 * 	Author: Mark Hocevar, DNVGL Fuel Fighter
 * 	Date: 	23.3.2016
 *
 * 	Header file used in all universal modules.
 * 	It includes basic data and configurations. 
 *
 *
 *
 ***************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>

#define LED_RED 	PB5
#define LED_GREEN 	PB6
#define LED_BLUE 	PB7

/********************************************************************
 * RGB led control
 *******************************************************************/
/**************
 * Init RGB led connected to PB5, PB6 and PB7
 ************ */
void RGB_init(void);

/*************
 * Turn on led.
 * 	@arg: pin number (only accepts pb5, pb6 or pb7)
 *************/
void RGB_led_on(int num);

/*************
 * Turn off led.
 * 	@arg: pin number (only accepts pb5, pb6 or pb7)
 *************/
void RGB_led_off(int num);

/*************
 * Toggle led.
 * 	@arg: pin number (only accepts pb5, pb6 or pb7)
 *************/
void RGB_toggle(int num);	
