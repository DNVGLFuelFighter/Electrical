/*
 * Main.h
 *
 * Created: 12.05.2013 18:38:40
 *  Author: Jostein
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#define WHEEL_CIRCUMFERENCE_MM 1750.0
#define MEASURE_POINTS 1

#include "can.h"

extern double vehicleSpeed;

/*********************************************************************************************************************
	Initialize the module
**********************************************************************************************************************/
void init_front_module(void);

/*********************************************************************************************************************
	Send data from speed sensor on CAN-bus
**********************************************************************************************************************/
void send_speed_data(double);


#endif /* MAIN_H_ */