/*
 * front_lights.c
 *
 * Created: 3/24/2016 12:35:00 PM
 *  Author: Vilius
 */ 

#include "front_lights_module.h"

BOOL eyebrows = FALSE;

void front_lights_init( void) {
	// Set PE4 and PE5 as outputs
	set_bit(DDRE, DDE4);
	set_bit(DDRE, DDE5);
	
	// Set ADC0, ADC1 and ADC2 as outputs
	set_bit(DDRF, DDF0);
	set_bit(DDRF, DDF1);
	set_bit(DDRF, DDF2);
	
	// Set respective PWM ports as outputs
	clear_bit(PORTE, DDE3);
	set_bit(DDRE, PE3);

	// Initialize 16 bit timer to use PWM
	set_bit_ex(TCCR3B, WGM33);
	
	// Set PWM output low on compare match for OC1A, OC1B, OC1C
	TCCR3A |= (1<<COM3A1) &~ (1<<COM3A0);

	// Set prescaler to clk/64
	set_bit(TCCR3B, CS31);
	set_bit(TCCR3B, CS30);
	
	// Set counter top value to 256
	ICR3L = 0xff;
	ICR3H =  0x00;

	// Set main lights off
	OCR3AL = 0x00;
}

void front_lights_headlights( int power) {
	if (power > HEADLIGHT_POWER_LIMIT) {
		OCR3AL = HEADLIGHT_POWER_LIMIT;
		return;
	}
	OCR3AL = power;
}

void front_lights_eyebrows( BOOL on) {
	eyebrows = on;
	if (on) {
		set_bit(PORTF, PF1);
		set_bit(PORTE, PE5);
		return;
	}
	clear_bit(PORTF, PF1);
	clear_bit(PORTE, PE5);
}

void front_lights_angel_eyes( BOOL on) {
	if (on){
		set_bit(PORTE, PE4);
		return;
	}
	clear_bit(PORTE, PE4);
}

void front_lights_turn_left( BOOL on) {
	if (on) {
		set_bit(PORTF, PF2);
		if (!eyebrows)
			set_bit(PORTF, PF1);
		return;
	}
	clear_bit(PORTF, PF2);
	if(eyebrows)
		clear_bit(PORTF, PF1);
}

void front_lights_turn_right( BOOL on) {
	if (on) {
		set_bit(PORTF, PF0);
		if (!eyebrows)
			set_bit(PORTE, PE5);
		return;
	}
	clear_bit(PORTF, PF0);
	if(eyebrows)
		clear_bit(PORTE, PE5);	
}

void light_show(CAN_packet *p, unsigned char mob) {
	printf("\r\nPrepare for a light show");
	(void)mob;
	
	if (p->data[0] == TRUE) {
		int cntr = 0;
		int off = -5;
		while(!off) {
			switch(cntr){
				case 1:
				front_lights_headlights(128);
				break;
				case 2:
				front_lights_eyebrows(TRUE);
				break;
				case 3:
				front_lights_turn_left(TRUE);
				break;
				case 4:
				front_lights_turn_right(TRUE);
				break;
				case 5:
				front_lights_angel_eyes(TRUE);
				break;
			}
			toggle_bit(DDRB, PB6);
			_delay_ms(100);
			cntr = cntr%5;
			cntr++;
			off++;
			front_lights_headlights(FALSE);
			front_lights_eyebrows(FALSE);
			front_lights_angel_eyes(FALSE);
			front_lights_turn_left(FALSE);
			front_lights_turn_left(FALSE);
		}
	}
}

void light_handler(CAN_packet *p, unsigned char mob) {
	printf("\r\nNew information on light module received");
	printf("\r\n----------------------------------------");
	(void)mob;
	
	/* Headlights near and far */
	if (p->data[0] == 0b00000001)
		front_lights_headlights(HEADLIGHT_POWER_LIMIT/2);
	else if (p->data[0] == 0b00000010)
		front_lights_headlights(HEADLIGHT_POWER_LIMIT);
	else front_lights_headlights(0);
	/* Right turn signal */
	if (p->data[0] == 0b00000100)
		front_lights_turn_right(TRUE);
	else front_lights_turn_right(FALSE);
	/* Left turn signal */
	if (p->data[0] == 0b00001000)
		front_lights_turn_left(TRUE);
	else front_lights_turn_left(FALSE);
	/* Angel eyes */
	if (p->data[0] == 0b00010000)
		front_lights_angel_eyes(TRUE);
	else front_lights_angel_eyes(FALSE);
	/* Eyebrows */
	front_lights_eyebrows(p->data[0] & 0b00100000);
}