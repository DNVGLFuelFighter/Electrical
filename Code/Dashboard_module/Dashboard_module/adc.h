/*
 * adc.h
 *
 * Created: 3/30/2016 4:13:20 PM
 *  Author: Vilius
 */ 
/*! Module for converting analog signal to digital values
	Primarily used by the steering wheel to read the
	analog values of the potentiometers that are used
	to set speed, fans etc.
	
	TODO: Look into ADC noise canceler (p. 280 on AT90CAN32/64/128)
	if the values received are garbage or not accurate enough.
*/

#ifndef ADC_H_
#define ADC_H_

#include "definitions.h"
#include <avr/io.h>
#include "can.h"

/*! Initialize 10-bit adc */
void adc_init( void);
/*! Put the adc to sleep when not needed to save power */
void adc_sleep( void);
/*! TODO: Write about the function
*/
void adc_input( CAN_packet*);


#endif /* ADC_H_ */