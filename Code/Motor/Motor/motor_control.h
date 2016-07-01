/*
 * Motor control 
 *
 * Created 03/04/2016
 * Author: Mark Hocevar
 * DNV GL Fuel Fighter 2016
 **
 *
 */
#include "definitions.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <util/delay.h>

#include "can.h"
#include "USART.h"
#include "universal_module.h"

#define MAXSPEED 	19999-2000
#define MINSPEED 	19999-1024
#define MAXPWM_CNT 	19999

#define BAUD_ESC 	9800 //TODO check if 9600 is ql
#define MYUBRR_ESC (F_CPU/16/BAUD_ESC)-1

typedef struct{
	uint8_t percentage; //Values 0-100 	 	=> 7bit
	uint8_t temp; 		//Values 0-100   	=> 7bit
	uint8_t voltage; 	//Values 12.0-50.0  =>  (with 0.1 precision) 
	uint16_t rpm; 		//values 0-10000 	=> 14bit (maybe 13bit is enough)
} esc;

extern uint16_t tar_speed;
extern uint16_t old_speed;

/* Init of motor control module  */
void init(void);
/* Init hardware PWM on timer 3, output compare B (pin PE4) */
/*
 * Timer3, output compare B, clear on match, set on top
 * Prescaler = 8Mhz / 8 (fuse) / 1 (timer prescaler)
 * mode = 15 (fast pwm with top on :q
 * ocr3a
 */
void pwm_init(void);
/* Init serial communication for telemetry */
void esc_telemetry_init(unsigned int baud);
/* Receive function for telemetry (serial port 1) */
void esc_telemetry_recv(void);
/* Decode function for screen of telemetry (data reveived with telemetry) */
void decode_screen(void);
/* Set speed (pwm value) of motors  */
void set_speed(uint16_t target);	
/* Function called on can receive package  */
void can_recv_cc(CAN_packet *p, unsigned char mob);
/* Send telemetry data from esc to can */
void can_send(void);
/* Timer for sending can message every x seconds */
void timer_init(void);

uint16_t PI(uint16_t curr_speed);





