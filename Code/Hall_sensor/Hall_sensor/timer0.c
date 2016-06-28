
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer0.h"
#include "main.h"

unsigned long milliseconds = 0;
unsigned int counter = 0;

int clk_ticks = 0;

void init_timer(void)
{
	TCCR3B = 0x02; //Set divider
	TCNT3 = 0xFC1A; //Apprx. 1ms to counter overflow
	TIMSK3 = 0x01; //Set counter overflow interrupt
	
    // Initialize 8bit Timer0.	
	TCCR0A = (1 << CS02);
	ENABLE_TM0OVF_INT; 
}

ISR(TIMER0_OVF_vect)
{
	cli();
	
	if (clk_ticks >= 120)
	{
		clk_ticks = 0;
	}
	else
	{
		clk_ticks++;
	}
	
	sei();
}

ISR(TIMER3_OVF_vect){
	cli();
	TCNT3 = 0xFC1A; //Apprx. 1ms to counter overflow
	milliseconds = milliseconds + 1;
	// Send CAN messages
	if (!(clk_ticks % 1000)) {
		send_speed_data(vehicleSpeed);
			
		//Debugging
		/*printString("Speeeeeeeed:");
		printDouble(vehicleSpeed, 1);
		printString("\n\r");*/
	}
	sei();
}

unsigned long millis(void){
	//cli();
	return milliseconds;
	//sei();
}
