#include <avr/io.h>
#include <avr/delay.h>
#include "lights.h"


int eyebrows = 0;

void init_lights(void)
{
	// Set PE4 and PE5 as outputs
	DDRE |= (1 << DDE4) | (1 << DDE5);
	
	// Set ADC0, ADC1 and ADC2 as outputs
	DDRF |= (1 << DDF0) | (1 << DDF1) | (1 << DDF2);
	
	
	// Timer/Counter 1 ..............................................
	
	// Set respective PWM ports as outputs
	PORTE &= ~(1<<DDE3);
	DDRE |= (1<<PE3);

	// Initialize 16 bit timer to use PWM
	TCCR3B = (1<<WGM33);
			
	// Set PWM output low on compare match for OC1A, OC1B, OC1C
	TCCR3A |= (1<<COM3A1) &~ (1<<COM3A0);

	// Set prescaler to clk/64
	TCCR3B |=  (1<<CS31) | (1<<CS30);
			
	// Set counter top value to 256
	ICR3L = 0xff;
	ICR3H =  0x00;

	// Set main lights off
	OCR3AL = 0x00;
}

void set_headlights(int power)
{
	if (power > 210) //limit output to 210, heat sinks are to small to handle 255
	{
		OCR3AL = 210;
	}
	else {
		OCR3AL = power;
	}
}

void set_eye_brows(int on)
{
	if (on){
		PORTF |= (1 << PF1);
		PORTE |= (1 << PE5);
	}
	else{
		PORTF &= ~(1 << PF1);
		PORTE &= ~(1 << PE5);
	}

	eyebrows = on;
}

void set_angel_eyes(int on)
{

	if (on){
		PORTE |= (1 << PE4);
	}
	else{
		PORTE &= ~(1 << PE4);
	}
}

void set_left_turn(int on)
{	
	if (on){
		PORTF |= (1 << PF2);
		if (!eyebrows){ //if eyebrows not on, toggle pin PF1 to complete the circuit
			PORTF |= (1 << PF1);
		}
	}
	else{
		PORTF &= ~(1 << PF2);
		if (!eyebrows){ //if eyebrows not on, toggle pin PF1 to complete the circuit
			PORTF &= ~(1 << PF1);
		}
	}
}

void set_right_turn(int on)
{
	if (on){
		PORTF |= (1 << PF0);
		if (!eyebrows){ //if eyebrows not on, toggle pin PE5 to complete the circuit
			PORTE |= (1 << PE5);
		}
		
	}
	else{
		PORTF &= ~(1 << PF0);
		if (!eyebrows){ //if eyebrows not on, toggle pin PE5 to complete the circuit
			PORTE &= ~(1 << PE5);
		}
	}
}
