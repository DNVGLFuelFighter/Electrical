/*
 * front_module.c
 *
 * Created: 3/31/2016 10:11:16 AM
 *  Author: Vilius
 */ 

#include "front_module.h"

void fm_init( void) {
	spi_init_master();
	
	set_bit(DDRD, DDD0);
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
	if(voltage > 12)
		voltage = 12.0;
	if (voltage < 3)
		/* Turn off the horn */
		clear_bit(PORTD, PD0);
	else {
		DAC_SELECT;
		spi_master_tx(OUT_E);
		spi_master_tx(fm_voltage_to_DAC(voltage));
		DAC_DESELECT;
		
		set_bit(PORTD, PD0);
	}
}

void fm_wiper(float voltage) {
	if(voltage > 12)
		voltage = 12.0;
	if (voltage < 3)
	/* Turn off the wipers */
	clear_bit(PORTE, PE4);
	else {
		DAC_SELECT;
		spi_master_tx(OUT_F);
		spi_master_tx(fm_voltage_to_DAC(voltage));
		DAC_DESELECT;
		
		set_bit(PORTE, PE4);
	}
}

void fm_fans(float voltage) {
	if(voltage > 12)
		voltage = 12.0;
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
	
	if (voltage >12)
		voltage = 12.0;
	int dac_value;
	dac_value = 255 - ((voltage - 1.5)*12.44);
	if(dac_value < 0)
		return 0;
	if(dac_value > 255)
		return 255;
	return dac_value;
}

void fm_brake( CAN_packet* p) {
	p->data[0] = ((PINE & (1 << PINE5)) >> PINE5); // what does this mean?
	/*
	if(!test_bit(PORTE, PE5)) {
		printf("\r\nPE5 is on");
		p->data[0] = 1;
	}
	else {
		printf("\r\nPE5 if off");
		p->data[0] = 0;
	}*/
}

void fm_handler(CAN_packet* p, unsigned char mob) {
	(void)mob;
	/*
	if(p->data[0] & (1<<4)) {
		printf("\r\nHORN");
		fm_horn(12);
	}
	else
		fm_horn(0);
	*/
	if (p->data[0] & (1<<5)) {
		printf("\r\nWIPER");
		fm_wiper(12);
	}
	else
		fm_wiper(0);
	
	// TODO: implement fans handler
	// WHERE DOES INPUT COME FROM?
}
