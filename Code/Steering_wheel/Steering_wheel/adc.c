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

void adc_input( CAN_packet* p) {
	/*Select the reference channel */
	ADMUX |= 0x00;
	/* Start the conversion */
	set_bit(ADCSRA, ADSC);
	/* Wait for the conversion to complete */
	while(test_bit(ADCSRA, ADSC));
	unsigned int full_value = (512 - ADC)/2;
	if(full_value >255)
		full_value = 255;
// 	double temp_val = 0.0;
// 	BOOL value_over_half = FALSE;
// 	temp_val = full_value/4.0;
// 	if(temp_val>=(floor(temp_val) + 0.5)){
// 			value_over_half = TRUE;
// 	}
// 	p->data[2] = (int)ceil(temp_val);
// 	p->data[3] = (int)floor(temp_val);
// 	p->data[4] = (int)floor(temp_val);
// 	p->data[5] = (int)floor(temp_val);
// 	if(value_over_half)
// 		p->data[3] = (int)ceil(temp_val);
	p->data[1] = (full_value>>2); // left shift to fit into 8-bit msg
}

/*! ADC conversion complete interrupt 
	Send a CAN message to the motor with new info 
*/
//ISR(ADC_vect) {}