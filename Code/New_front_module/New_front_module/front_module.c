/*
 * front_module.c
 *
 * Created: 4/8/2016 5:55:16 PM
 *  Author: Vilius
 */ 

#include "front_module.h"

BOOL HORN_ON = FALSE;
BOOL WIPERS_ON = FALSE;
BOOL FANS_ON = FALSE;
BOOL CW = TRUE;
BOOL CCW = FALSE;
int duty = 1500;

void fm_msg_handler(CAN_packet* p, unsigned char mob) {
	(void)mob;
	printf("\r\np->data[1] %u", p->data[1]);
	switch(p->id) {
		case ID_steeringWheel:
			if(p->data[0] & (1<<4))
				HORN_ON = TRUE;
			else
				HORN_ON = FALSE;
			if (p->data[0] & (1<<5))
				WIPERS_ON = TRUE;
			else
				WIPERS_ON = FALSE;
			break;
		case ID_dashboard:
			if(p->data[0] & (1<<7))
				FANS_ON = TRUE;
			else
				FANS_ON = FALSE;
	}
}

void fm_horn_handler( void) {
	if(HORN_ON) {
		fm_horn(11); // 12 VDC
	} else {
		fm_horn(0);
	}
}

void fm_wipers_handler( void) {
	if(WIPERS_ON) {
		fm_wipers(5); // 6 VDC
		if(CW) {
			CCW = FALSE;
			if(duty < 2200) {
				duty = duty+10;
			} else {
				CW = FALSE;
				CCW = TRUE;
			}
		}
		if (CCW) {
			CW = FALSE;
			if(duty > 1500) {
				duty = duty-10;
			} else {
				CW = TRUE;
				CCW = FALSE;
			}
		}	
		fm_wipers_duty(duty);	
	} else
		fm_wipers(0);
}

void fm_wipers_duty(int duty) {
	if(duty > 800 && duty <2200) {
		cli();
		OCR3B = 3500 - duty;
		sei();
	}
}

void fm_fans_handler( void) {
	fm_fans(FANS_ON);
}

void fm_brake_watcher(CAN_packet* msg_old, CAN_packet* msg_new){
	BOOL ret;
	msg_new->data[0] = 0;
	if(test_bit(PINE, PINE5))
		msg_new->data[0] = 1;
	int different = memcmp(msg_new->data, msg_old->data, 8);
	if(different) {
		ret = can_packet_send(0, msg_new);
		msg_old = msg_new;
	} else
		ret = FALSE;
	if(ret) {
		set_bit(DDRB, PB7);
	}
	else
		clear_bit(DDRB, PB7);
}

void fm_horn_init( void) {
	/* PD0 - DC0_EN, PE4 -DC1_EN, PE3 - DC2_EN */
	set_bit(DDRD, DDD0);
	set_bit(DDRE, DDE3);
	set_bit(DDRE, DDE4);
	/* Brake pedal with pull up*/
	clear_bit(DDRE, DDE5);
}

void fm_wiper_init( void) {
	/* Set PB4 as output */
	set_bit(DDRB, DDB4);
	clear_bit(PORTB, PB4);
}

void fm_fans_init( void) {
	//TODO: IMPLEMENT
}

void fm_init( void) {
	fm_horn_init();
	fm_wiper_init();
	fm_fans_init();
}

void fm_horn(float voltage) {
	if (voltage > 12) {
		voltage = 12;
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

void fm_fans(float voltage) {
// 	if(voltage < 5) {
// 		clear_bit(PORTE, PE3);
// 		return;
// 	}
// 	float val = voltage;
// 	// low - 215, high - 141
// 	if(val < 80)
// 		val = 80;
// 	val = 210-0.39*(val-80);
// 	
	if (!voltage) {
		/* Turn off the fans */
		clear_bit(PORTE, PE3);
	} else {
		voltage = 141; // 0=24 VDC, 255=0VDC, 141=12VDC
		DAC_SELECT;
		spi_master_tx(OUT_G);
		spi_master_tx(voltage);
		DAC_DESELECT;
		
		set_bit(PORTE, PE3);
	}
}

void fm_wipers(float voltage) {
	if(voltage > 6) {
		voltage = 7.4;
	}
	if (voltage < 2) {
		/* Turn off the wipers */
		clear_bit(PORTE, PE4);
	} else {
		DAC_SELECT;
		spi_master_tx(OUT_F);
		spi_master_tx(fm_voltage_to_DAC(voltage));
		DAC_DESELECT;
		
		set_bit(PORTE, PE4);
	}
}

int fm_voltage_to_DAC(float voltage) {
	int dac_value = 0;
	dac_value = 255 - (255*voltage/24); //TODO: tune this
	
	return dac_value;
}