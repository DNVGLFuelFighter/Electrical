/*
 * LED.c
 *
 * Created: 3/19/2016 12:25:21 PM
 *  Author: Vilius
 */ 
#include "definitions.h"
#include <avr/io.h>
#include <util/delay.h>



#define BLUE	PB7
#define RED		PB5
#define GREEN	PB6

void LED_police(_Bool onOff) {
	const uint32_t delay = 1000;
	if (onOff == 1) {
		_delay_ms(delay);
		set_bit_ex(DDRB, BLUE);
		_delay_ms(delay);
		clear_bit(DDRB, BLUE);
		set_bit(DDRB, RED);
		set_bit(DDRB, BLUE);
		set_bit(DDRB, GREEN);
		_delay_ms(delay);
		set_bit_ex(DDRB, RED);
	}
}
