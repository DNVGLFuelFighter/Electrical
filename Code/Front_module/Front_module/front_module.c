/*
 * front_module.c
 *
 * Created: 3/31/2016 10:11:16 AM
 *  Author: Vilius
 */ 

#include "front_module.h"

BOOL HORN_ON =	FALSE;
BOOL WIPERS_ON = FALSE;
BOOL FANS_ON = FALSE;

void fm_init( void) {
	spi_init_master();
	
	set_bit(DDRD, DDE0);
	set_bit(DDRE, DDE3);
	set_bit(DDRE, DDE4);
	/* Brake pedal with pull up*/
	clear_bit(DDRE, DDE5);
	//clear_bit(PORTE, PE5);
	fm_fans(0);
	fm_horn(0);
	fm_wiper(0);
}

void fm_horn(float voltage) {
	if (voltage > 12) {
		voltage = 12;
		set_bit(PORTD, PD0);
	}
	if (voltage <= 3) {
		/* Turn off the horn */
		voltage = 0;
		clear_bit(PORTD, PD0);
	} else {
		DAC_SELECT;
		spi_master_tx(OUT_E);
		spi_master_tx(fm_voltage_to_DAC(voltage));
		DAC_DESELECT;
		
		set_bit(PORTD, PD0);
	}
}

void fm_wiper(float voltage) {
	if(voltage > 12) {
		voltage = 12;
		set_bit(PORTE, PE4);
	}
	if (voltage < 3) {
	/* Turn off the wipers */
		voltage = 0;
		clear_bit(PORTE, PE4);
	} else {
		DAC_SELECT;
		spi_master_tx(OUT_F);
		spi_master_tx(fm_voltage_to_DAC(voltage));
		DAC_DESELECT;
		
		set_bit(PORTE, PE4);
	}
}

void fm_fans(float voltage) {
	if(voltage > 12)
		voltage = 12;
	if (voltage < 3)
	/* Turn off the fans */
		clear_bit(PORTE, PE3);
	else {
		DAC_SELECT;
		spi_master_tx(OUT_G);
		spi_master_tx(fm_voltage_to_DAC(voltage));
		DAC_DESELECT;
		
		set_bit(PORTE, PE3);
	}
}
// Last years function
int fm_voltage_to_DAC(float voltage) {
	int dac_value = 0;

	if(voltage == 12)
		dac_value = 130;
	return dac_value;
}

void fm_brake( CAN_packet* p) {
	p->data[0] = 0;
	if(test_bit(PINE, PINE5))
		p->data[0] = 1;
}

void fm_handler(CAN_packet* p, unsigned char mob) {
	(void)mob;
	
  	if(p->data[0] & (1<<4)) 
 		HORN_ON = TRUE;
  	else
 		HORN_ON = FALSE;
		
	if (p->data[0] & (1<<5)) {
		WIPERS_ON = TRUE;
	} else
		WIPERS_ON = FALSE;
			
	// TODO: implement fans handler
	// WHERE DOES INPUT COME FROM?
}
