/*
 * Hall_sensor.c
 *
 * Created: 27.02.2015
 * Author: Bjarne Kvæstad
 * Edited: 6/18/2016 1:02:10 PM
 * Author: Vilius Ciuzelis
 */ 

#include "definitions.h"

#include <avr/interrupt.h>
#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#include "main.h"
#include "UART_USB.h"
#include "minprintf.h"
#include "RGB_driver.h"
#include "can.h"
#include "timer0.h"

volatile uint32_t hallStopWatch, cHallTime;
volatile int cHallFlagChange;
/* !Physical implementation acts as if each revolution around axis is two ticks.
*/
volatile int magnet_count = 0;
double vehicleSpeed = 0;

//PE5 interrupt 
ISR(INT5_vect){
	magnet_count = magnet_count+1;
	cHallTime = millis() - hallStopWatch;
	hallStopWatch = millis();
	if (magnet_count >1) {
		magnet_count = 0;
		cHallFlagChange = 1;
	}
}

int main(void)
{
	init_front_module();
	unsigned long speedTimeout = 0;
	
    while(1)
    {
		uint32_t hallTime;
		//Get interrupt data
		if (cHallFlagChange){
			//set_RGB(255, 255, 0);
			cli();
			hallTime = cHallTime;
			cHallFlagChange = 0;
			speedTimeout = millis();
			sei();
			
			//calculate speed, in mm/ms (aka m/s)
			vehicleSpeed = (WHEEL_CIRCUMFERENCE_MM)/(double)(MEASURE_POINTS * hallTime);
			
			printInt(hallTime,2);
			printString("\n\r");
// 			
// 			set_RGB(0, 255, 0);
		}
		
		//Set speed to zero if no magnet has passed the sensor within 2s.
		if (speedTimeout + 2000 < millis()){
			vehicleSpeed = 0.0;
		}
    }
}


void send_speed_data(double vehicle_speed)
{
	CAN_packet send;
	
		
	uint16_t toSend = (uint16_t)(vehicle_speed*100);
	send.id = ID_hallSensor;
	send.length = 2;
		
	send.data[1] = toSend & 0xff;
	send.data[0] = (toSend >> 8) & 0xff;
		
	cli();
	
	can_tx(0x01, &send);
	sei();
		
}


void init_front_module( void )
{
	//internal pull-up resistor
	PORTE |= (1 << PE5);
	
	//Set interrupt, change HIGT to LOW and LOW to HIGH on PE5
	EICRB = (1 << 2);
	EIMSK = (1 << 5);
	
	init_USB_USART();
	init_RGB_driver();
	can_init();
	init_timer();
	set_RGB(0, 255, 0);
				
	sei();	
}
