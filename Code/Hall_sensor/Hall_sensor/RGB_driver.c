#include <avr/io.h>
#include "RGB_driver.h"



void init_RGB_driver()
{
		// Timer/Counter 1 ..............................................
		
		// Set respective PWM ports as outputs
		PORTB &= ~((1<<PB5)|(1<<PB6)|(1<<PB7));
		DDRB |= (1<<PB5)|(1<<PB6)|(1<<PB7);

		// Initialize 16 bit timer to use fast PWM
		TCCR1A = (1<<WGM11) &~(1<<WGM10);
		TCCR1B = (1<<WGM12) | (1<<WGM13);
		
		// Set PWM output low on compare match for OC1A, OC1B, OC1C
		TCCR1A |= (1<<COM1A1) &~ (1<<COM1A0);
		TCCR1A |= (1<<COM1B1) &~ (1<<COM1B0);
		TCCR1A |= (1<<COM1C1) &~ (1<<COM1C0);

		// Set prescaler to clk/256
		TCCR1B |= (1<<CS12) &~ (1<<CS11) &~ (1<<CS10);
		
		// Set counter top value to 256
		ICR1H = 0;
		ICR1L =  0xff;

		// Set off
		OCR1AL = 0xFF;
		OCR1BL = 0xFF;
		OCR1CL = 0xFF;
}


void set_RGB( int R, int G, int B )
{
	OCR1AL = (0xFF - R);
	OCR1BL = (0xFF - G);
	OCR1CL = (0xFF - B);
}
