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
int DIR_CNT = FALSE;

void fm_init( void) {
	fm_horn_init();
	fm_wiper_init();
	fm_fans_init();
	spi_init_master();
}

void fm_horn_init( void) {
	set_bit(DDRD, DDD0);
	set_bit(DDRE, DDE3);
	set_bit(DDRE, DDE4);
	/* Brake pedal with pull up*/
	clear_bit(DDRE, DDE5);
	//clear_bit(PORTE, PE5);
	fm_horn(0);
}

void fm_wiper_init( void) {
	/* Set PB4 as output */
	set_bit(DDRB, DDB4);
	fm_wiper(0);
}

void fm_fans_init( void) {
	fm_fans(0);
}

void fm_horn(float voltage) {
	if (voltage > 12) {
		voltage = 12;
		//set_bit(PORTD, PD0);
	}
	if (voltage < 3) {
		/* Turn off the horn */
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
	if(voltage > 6) {
		voltage = 7.4;
	}
	if (voltage < 2) {
	/* Turn off the wipers */
		clear_bit(PORTE, PE4);
	} else {
		DAC_SELECT;
		spi_master_tx(OUT_F);
		//printf("\r\nfm_voltage_to_ADC = %d", (int)fm_voltage_to_DAC(voltage));
		spi_master_tx(fm_voltage_to_DAC(voltage));
		//spi_master_tx(255);
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
	dac_value = 255 - (255*voltage/24);
	/*if(voltage == 12)
		dac_value = 130;
	else if (voltage == 7.4)
		dac_value = 70;
	else if (voltage == 6)
		dac_value = 220;*/
	
	return dac_value;
}

void fm_brake( CAN_packet* p) {
	p->data[0] = 0;
	if(test_bit(PINE, PINE5))
		p->data[0] = 1;
}

void fm_handler(CAN_packet* p, unsigned char mob) {
	(void)mob;

  	if(p->data[0] & (1<<4)) {
 		HORN_ON = TRUE;
		 printf("\r\nTurn on horn");
	}
  	else
 		HORN_ON = FALSE;
		
	if (p->data[0] & (1<<5)) {
		WIPERS_ON = TRUE;
		printf("\r\nTurn on wipers");
	}
	else 
		WIPERS_ON = FALSE;
	
	// TODO: implement fans handler
	// WHERE DOES INPUT COME FROM?
}

void fm_wiper_routine( void) {
	if(test_bit(PORTB, PB5))
		set_bit(PORTB, PB4);
	else
		clear_bit(PORTB, PB4);
	int value;
	value = 1024 + 1024*DIR_CNT/5;
	OCR1B = 19999 - value;
	if (DIR_CNT >= 5)
		DIR_CNT = 0;
}