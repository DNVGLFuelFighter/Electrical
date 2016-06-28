/*********************************************************************************************************************

	Timer for uppdateing system CLK ticks            
                                                                                                   
	Author:		Jostein Furseth						18.05.2013					                

**********************************************************************************************************************/


#ifndef TIMER0_H_
#define TIMER0_H_

#define ENABLE_TM0OVF_INT TIMSK0 |= (1<<TOIE0)
#define DISABLE_TM0OVF_INT TIMSK0 &= ~(1<<TOIE0)
#include "minprintf.h"


/*********************************************************************************************************************
	Initialize timer 0 to overflow approx 122 times/sec.
**********************************************************************************************************************/
void init_timer(void);
unsigned long millis(void);


#endif /* TIMER0_H_ */