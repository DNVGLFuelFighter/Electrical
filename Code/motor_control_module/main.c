/*
 * Motor control with pwm to esc
 *
 * Created: 3/94/2016 
 * Author : Mark Hocevar
 */ 

#include <avr/interrupt.h>
#include "can.h"
#include "USART.h"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "universal_module.h"

void init(void);
void pwm_init(void);
void can_recv(CAN_packet *p, unsigned char mob);


uint16_t speed = 0; //10bit
uint16_t max_cnt = 19999; //Period of pwm (19999 -> 20ms)

int main(void){
	init();
	pwm_init();
	RGB_init();
	RGB_led_on(LED_BLUE);
	_delay_ms(5000);
	
/*	while(1){
		RGB_led_off(LED_BLUE);
		RGB_led_on(LED_GREEN);
		for(int i = 0; i < 300; i+=10){
			speed = 19999 - (1024 + i);
			OCR3B = speed;
			_delay_ms(100);
		}
		RGB_led_off(LED_GREEN);
		RGB_led_on(LED_RED);	
		_delay_ms(1000);
		for(int i = 300; i > 0; i-=10){
			speed = 19999 - (1324 - i);
			OCR3B = speed;
			_delay_ms(100);
		}
		RGB_led_off(LED_RED);
		RGB_led_on(LED_BLUE);
		break;*/
	}
	while(1){}
}

void init(void){
	can_init();
	USART_init(MYUBRR, true);
	printf("Init complete\n\r");
	//TODO id of package and mask
	prepare_rx(0, 14, 0xff, can_recv);
}

void can_recv(CAN_packet *p, unsigned char mob){
	//Parse data from message
	//TODO do some checking of data so we don't get stupid stuff
	uint16_t recv = (p->data[0] << 8) | p->data[1] ;
	speed = max_cnt - (1024 + recv);
	printf("Recv data %d\n\r", speed);
	//TODO make some stepping so it won't change to harsh
	//OCR3B = speed;
}

void pwm_init(){
	/*
	 * Timer3, output compare B, clear on match, set on top
	 * Prescaler = 8Mhz / 8 (fuse) / 1 (timer prescaler)
	 * mode = 15 (fast pwm with top on ocr3a
	 * */
	TCCR3A = (1 << COM3B1) | (1 << COM3B0) | (1 << WGM31) | (1 << WGM30);
	TCCR3B = (1 << WGM33) | (1 << WGM32) ;
	TCCR3C = 0;

	uint16_t duty = max_cnt - 1024;
	OCR3A = max_cnt; 
	OCR3B = duty;
	
	DDRE |= (1 << DDE4); //set pin as output

	TCCR3B |= (1<<CS31); //start timer with prescaler 1;
}
