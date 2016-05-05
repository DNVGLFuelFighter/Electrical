/*
 * Power_measure_module.c
 *
 * Created: 09.04.2015 21:41:32
 *  Author: olestokk
 */ 


#include "definitions.h"
#include "SPI.h"
#include "USART.h"
#include "can.h"
#include <stdio.h>
#include <string.h>

#define LED1 PD3
#define LED2 PD2 
#define LED3 PD1
#define E_STOP PE3


//TODO change ids
#define E_STOP_ID	1
#define MEASURE_ID	20
#define MOTOR_STATUS_ID	12

//this is calibrated from measuring with multimeter
#define VOLTAGE_SCALE	513
#define CURRENT_MOTOR_ZERO	2092
#define CURRENT_SOLAR_ZERO	520
#define CURRENT_MOTOR_SCALE 104
#define CURRENT_SOLAR_SCALE 73

uint8_t spi_seq;
uint8_t spi_ch = 0;
volatile uint16_t counter = 0;

uint16_t voltage_ADC = 0; 
uint16_t current_motor_ADC = 0;


volatile uint32_t voltage_ADC_sum = 0;
volatile uint32_t current_motor_ADC_sum = 0;
volatile uint32_t current_motor_one_value = 0;

volatile bool timer_done = false;

uint16_t Ext_ADC_read(uint8_t channel, bool single_ended)
{
	uint16_t value;
	
	PORT_SPI &= ~(1<<DD_SS);
	SPDR = (0x04 | (single_ended<<1));
	while(!(SPSR & (1<<SPIF)));
	
	SPDR = channel<<6;
	while(!(SPSR & (1<<SPIF)));
	
	value = ((SPDR & 0x0F)<<8);
	
	SPDR = 0;
	while(!(SPSR & (1<<SPIF)));
	
	value |= SPDR;
	
	PORT_SPI |= (1<<DD_SS);
	
	return value;
}

bool sat_on_latch = false;
volatile bool motor_on = false;

void receiveEStop( CAN_packet *p, unsigned char mob) {
	(void)mob;
	
	uint8_t data = p->data[2];
	printf("Hey i am in receiveEStop\n\r");	
	if (data < 147)
	{
		printf("Turn off propulsion\n\r");
		//turn off propulsion system
		PORTE |= (1<<E_STOP);
		PORTD &= ~(1<<LED2);
		sat_on_latch = false;
		motor_on = false;
	}
	else if (data > 177)
	{
		if (!sat_on_latch)
		{
			printf("Turn on propulsion\n\r");
			//turn on propulsion system
			PORTE &= ~(1<<E_STOP);
			PORTD |= (1<<LED2);
			motor_on = true;
		}
	}
}

void receiveMotorStatus( CAN_packet *p, unsigned char mob) {
	(void)mob;
	
	uint8_t data = p->data[2];
	
	if (data == 1)
	{
		//turn off propulsion system
		PORTE |= (1<<E_STOP);
		motor_on = false;
		PORTD &= ~(1<<LED2);
		sat_on_latch = true;
	}
	else if (data == 0)
	{
		
		if (!sat_on_latch)
		{
			//turn on propulsion system
			PORTE &= ~(1<<E_STOP);
			PORTD |= (1<<LED2);
			motor_on = true;
		}
	}
}

int main(void)
{
	//USART init (mark)
	USART0_Init(MYUBRR0);
	fdevopen(uart_putc0, USART0_Receive);
	printf("Hello World!\n");
	
	//SPI_MasterInit();
	//SPI_config for spi adc module ??? (mark)
	DDR_SPI |= (1<<DD_MOSI) | (1<<DD_SCK) | (1<<DD_SS);
	DDR_SPI &= ~(1<<DD_MISO);
	PORT_SPI |= (1<<DD_SS);
	SPCR = (1<<SPIE) | (1<<SPE) | (1<<MSTR);
	//Can init with id 20. (mark)
	can_init();
	CAN_packet p;
	p.id = MEASURE_ID;
	p.length = 6;
	//Estop can message recv funcion setup -> id 1
	prepare_rx(1, E_STOP_ID, 0x7ff, receiveEStop);
	//prepare_rx(2, MOTOR_STATUS_ID, 0x7ff, receiveMotorStatus);
	
	
	//leds and e_stop relay
	DDRD |= (1<<LED1) | (1<<LED2) | (1<<LED3);
	DDRE |= (1<<E_STOP);
	
	PORTD &= ~((1<<LED1) | (1<<LED2) | (1<<LED3));
	PORTE &= ~(1<<E_STOP);
	
	//ADC1 positive differential input, ADC0 negative differential input
	//ADMUX = (1<<REFS0) | (1<<REFS1) | 0x10;
	//Enable, start conversion, interrupt enable, prescaler 64 (8MHz/64 = 125KHz)
	//ADCSRA |= (1<<ADEN) | (1<<ADSC) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1);
	
	TCCR1B |= (1<<WGM12) | (1<<CS12);		//CTC mode, prescaler 256
	TIMSK1 |= (1<<OCIE1A);				//compare match A interrupt
	OCR1A = 31249;
	
	PORT_SPI &= ~(1<<DD_SS);	//initate new reading
	SPDR = 0x06;	//single ended
	spi_seq = 1;
	spi_ch = 0;
	
	sei();
	
	
	
	uint32_t temp_voltage, temp_current_motor;
	uint16_t temp_counter;
	
	for (unsigned char i = 0; i < p.length; i++)
	{
		p.data[i] = i;
	}
	
    while(1)
    {
		if (timer_done == true)
		{
			timer_done = false;
			//heartbeat (mark)
			PORTD ^= (1<<LED1);
			
			//saving sampled data (mark)
			cli();
			temp_voltage = voltage_ADC_sum;
			temp_current_motor = current_motor_ADC_sum;

			temp_counter = counter;
			voltage_ADC_sum = 0;
			current_motor_ADC_sum = 0;

			counter = 0;
			sei();
			
			//Average and scale data (mark)
			int voltage = temp_voltage*100/temp_counter/VOLTAGE_SCALE;
			uint32_t currentM = ((uint32_t)(temp_current_motor/temp_counter) - CURRENT_MOTOR_ZERO + 5)/CURRENT_MOTOR_SCALE;

 		
			//What is 455??? (mark)
			if (voltage > 455)
			{
				PORTD |= (1<<LED3);
			}
			else
			{
				PORTD &= ~(1<<LED3);
			}
			
			//Preapere can message (mark)
			p.data[0] = voltage>>8;
			p.data[1] = voltage;
			p.data[2] = currentM>>8;
			p.data[3] = currentM;
			p.data[4] = 0;
			p.data[5] = motor_on;
			
			//For debugging
			
			printf("Average over %d samples:\n\r", temp_counter);
			//printf("Voltage: %d\n\r", voltage);
			printf("Current motor: %u\n\r", currentM);
			printf("Current without scaling: %u\n\r", (uint32_t)(temp_current_motor/temp_counter));
			//printf("calc: (%u/%d - %d + 5)/%d*100)\n\r", temp_current_motor, temp_counter, CURRENT_MOTOR_ZERO, CURRENT_MOTOR_SCALE);
			printf("Current one: %d\n\r", current_motor_one_value);
			
			//printf("CAn packet: %s\n", str);
			can_tx(14, &p);
			
			PORTD ^= (1<<LED1); //heartbeat (mark)
		}
		
	}
}

ISR(TIMER1_COMPA_vect)
{
	timer_done = true;
}

//SPI transfer complete interrupt for communication with ADC
ISR(SPI_STC_vect)
{
	switch(spi_seq)
	{
		case 0:
			
			spi_seq++;
			PORT_SPI |= (1<<DD_SS);		//end the last reading
			_delay_us(1);	//necessary? how low can the pulse length be?
			PORT_SPI &= ~(1<<DD_SS);	//initate new reading
			
			switch(spi_ch)
			{
				case 0:
					current_motor_ADC |= SPDR;	//capture the last value
					current_motor_ADC_sum += current_motor_ADC;
					current_motor_one_value = current_motor_ADC;
					//initiate for next transfer
					spi_ch = 2;
					SPDR = 0x04;	//single ended
					break;
				case 2:
					counter++;
					voltage_ADC |= SPDR;	//capture the last value
					voltage_ADC_sum += voltage_ADC;
					
					//initiate for next transfer
					spi_ch = 0;
					SPDR = 0x06;	//single ended
					break;
			}
			break;
			
		case 1:
			spi_seq++;
			SPDR = spi_ch<<6;
			break;
			
		case 2:
			spi_seq = 0;
			switch(spi_ch)
			{
				case 0:
					current_motor_ADC = ((SPDR & 0x0F)<<8);
					break;
				case 2:
					voltage_ADC = ((SPDR & 0x0F)<<8);
					break;	
			}
			
			SPDR = 0;
			break;
	}
}
