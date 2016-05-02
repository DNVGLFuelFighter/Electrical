/*
 * front_lights.c
 *
 * Created: 3/24/2016 12:35:00 PM
 *  Author: Vilius
 */ 

#include "front_lights_module.h"

BOOL EMERG = FALSE;
BOOL IND_LEFT = FALSE;
BOOL IND_RIGHT = FALSE;
BOOL EYEBROWS_ON = FALSE;
BOOL SHORT_LIGHT = FALSE;
BOOL FAR_LIGHT = FALSE;

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
	if (on && !IND_LEFT && !IND_RIGHT) {
		set_bit(PORTF, PF1);
		set_bit(PORTE, PE5);
		EYEBROWS_ON = TRUE;
		return;
	}
	clear_bit(PORTF, PF1);
	clear_bit(PORTE, PE5);
	EYEBROWS_ON = FALSE;
}

void front_lights_angel_eyes( BOOL on) {
	if (on) {
		set_bit(PORTE, PE4);
		return;
	}
	clear_bit(PORTE, PE4);
}

void front_lights_turn_left( BOOL on) {
	if (on) {
		set_bit(PORTF, PF2);
		if (!EYEBROWS_ON)
			set_bit(PORTF, PF1);
		return;
	}
	clear_bit(PORTF, PF2);
	if(!EYEBROWS_ON)
		clear_bit(PORTF, PF1);
}

void front_lights_turn_right( BOOL on) {
	if (on) {
		set_bit(PORTF, PF0);
		if (!EYEBROWS_ON)
			set_bit(PORTE, PE5);
		return;
	}
	clear_bit(PORTF, PF0);
	if(!EYEBROWS_ON)
		clear_bit(PORTE, PE5);	
}

void front_light_handler(CAN_packet *p, unsigned char mob) {
	(void)mob;
	
	if (p->id == ID_dashboard) {
		/* Headlights near and far */
		if (p->data[0] & HEADLIGHTS_NEAR) {
			front_lights_headlights(30);
			SHORT_LIGHT = TRUE;
			FAR_LIGHT = FALSE;
		} else if (p->data[0] & HEADLIGHTS_FAR) {
			front_lights_headlights(150);
			SHORT_LIGHT = FALSE;
			FAR_LIGHT = TRUE;
		} else {
			front_lights_headlights(0);
			SHORT_LIGHT = FALSE;
			FAR_LIGHT = FALSE;
		}
		/* Eyebrows */
		front_lights_eyebrows(p->data[0] & EYEBROWS);
		/* Angel eyes */
		front_lights_angel_eyes(p->data[0] & ANGEL_EYES);
		/* Emergency lights */
		front_emergency(p->data[0] & EMERGENCY);
	} else if (p->id == ID_steeringWheel) {
		/* Right turn signal */
		front_ind_right(p->data[0] & INDICATOR_RIGHT);
	
		/* Left turn signal */
		front_ind_left(p->data[0] & INDICATOR_LEFT);
	}
}

void front_toggle_ind_left( void) {
	if (get_ind_left())
		front_lights_turn_left(FALSE);
	else
		front_lights_turn_left(TRUE);
}

void front_toggle_ind_right( void) {
	if (get_ind_right())
		front_lights_turn_right(FALSE);
	else
		front_lights_turn_right(TRUE);
}

void front_emergency( BOOL on) {
	EMERG = on;
}

void front_ind_left( BOOL on) {
	if(on)
		if(!IND_LEFT)
			IND_LEFT = TRUE;
	else
		if(IND_LEFT)
			IND_LEFT = FALSE;
}

void front_ind_right( BOOL on) {
	if(on)
		if(!IND_RIGHT)
			IND_RIGHT = TRUE;
	else
		if(IND_RIGHT)
			IND_RIGHT = FALSE;
}

BOOL get_ind_left( void) {
	return test_bit(PORTF, PF2);
}

BOOL get_ind_right( void) {
	return test_bit(PORTF, PF0);
}

BOOL get_eyebrows( void) {
	return test_bit(PORTE, PE5);
}

BOOL get_angel( void) {
	return test_bit(PORTE, PE4);
}

void get_light_status(CAN_packet* p) {
	p->data[0] = 0;
	if(SHORT_LIGHT)
		p->data[0] |= (1<<0);
	if(FAR_LIGHT)
		p->data[0] |= (1<<1);
	if(get_ind_left)
		p->data[0] |= (1<<2);
	if(get_ind_right())
		p->data[0] |= (1<<3);
	if(get_eyebrows())
		p->data[0] |= (1<<4);
	if(get_angel())
		p->data[0] |= (1<<5);
	if(EMERG)
		p->data[0] |= (1<<6);
}

void light_show() {
	printf("\r\nPrepare for a light show");
	int cntr = 0;
	int off = 5;
	while(off) {
		switch(cntr){
			case 1:
			front_lights_headlights(50);
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
		clear_bit(DDRB, PB5);
		_delay_ms(5000);
		cntr = cntr%5;
		cntr++;
		off--;
		front_lights_headlights(FALSE);
		front_lights_eyebrows(FALSE);
		front_lights_angel_eyes(FALSE);
		front_lights_turn_left(FALSE);
		front_lights_turn_right(FALSE);
	}
}
