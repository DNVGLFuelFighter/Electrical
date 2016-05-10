/*
 * Gyro.c
 *
 * Created: 4/16/2016 10:46:39 AM
 * Author : Vilius
 */ 

#include "definitions.h"
#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include "USART.h"
#include "gyro.h"
//#include "twi.h"
#include "can.h"

int main(void)
{
	USART_init(MYUBRR, TRUE);
	gyro_init();
	CAN_packet p;
	p.id = 404;
	p.length = 4;
	p.data[0] = 0;
	adc_init();
	adc_input(1, &p);
    for(;;) {
		_delay_ms(100);
		adc_init();
// 		adc_input(1, &p);
// 		_delay_ms(100);
// 		adc_input(2, &p);
		_delay_ms(100);
		adc_input(3, &p);
		adc_sleep();
		for(int i=0;i<p.length;i++) {
			_delay_ms(100);
			printf("\r\np.data[%d] = %d", i, p.data[i]);
		}
		printf("\n");
		
		asm("sleep");
	}
}

