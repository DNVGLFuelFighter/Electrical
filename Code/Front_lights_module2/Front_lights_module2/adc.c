/*
 * adc.c
 *
 * Created: 3/30/2016 4:13:28 PM
 *  Author: Vilius
 */ 
#include "adc.h"

void adc_init( void) {
	clear_bit(DDRF, DDF0);
	
	/* Voltage ref AVcc with external capacitor on AREF pin */
	set_bit(ADMUX, REFS0);
	/* Select prescaler to 64 --> conversion f= 125kHz */
	set_bit(ADCSRA, ADPS2);
	set_bit(ADCSRA, ADPS1);
	/* Enable the ADC */
	set_bit(ADCSRA, ADEN);
	/* Turn on the ADC Conversion Complete Interrupt */
	//set_bit(ADCSRA, ADIE);
}

void adc_sleep( void) {
	/* Wait for the conversion to complete */
	while(test_bit(ADCSRA, ADSC));
	/* Turn off the ADC */
	clear_bit(ADCSRA, ADEN);
}

void adc_input(int ref, CAN_packet* p) {
	/*Select the reference channel */
	if (ref==SPEED)
		ADMUX |= 0x00;
	else if (ref==FANS)
		ADMUX |= 0x01;
	/* Start the conversion */
	set_bit(ADCSRA, ADSC);
	/* Wait for the conversion to complete */
	while(test_bit(ADCSRA, ADSC));
	p->data[1] = ADC;
}

/*! ADC conversion complete interrupt 
	Send a CAN message to the motor with new info 
*/
//ISR(ADC_vect) {}