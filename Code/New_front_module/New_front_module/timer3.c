/*
 * timer3.c
 *
 * Created: 4/7/2016 2:16:32 PM
 *  Author: Vilius
 */ 

#include "timer3.h"

void timer3_init( void) {
	/*
	* Timer3, output compare B, clear on match, set on top
	* Prescaler = 8 MHz/ 1 fuse / 8 prescaler
	* mode = 15 fast pwm with top on ocr3a
	* */
	TCCR3A = (1 << COM3B1) | (1 << COM3B0) | (1 << WGM31) | (1 << WGM30);
	TCCR3B = (1 << WGM33) | (1 << WGM32);
	TCCR3C = 0;
	
	uint16_t duty = 3500 - 1500;
	OCR3A = 3500;
	OCR3B = duty;
	
	DDRE |= (1<<DDE4);
	
	TCCR3B |= (1<<CS31);
}
