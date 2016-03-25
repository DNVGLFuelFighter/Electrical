/*
 * frontl_lights_module.h
 *
 * Created: 3/24/2016 12:35:30 PM
 *  Author: Vilius
 */ 


#ifndef FRONTL_LIGHTS_MODULE_H_
#define FRONTL_LIGHTS_MODULE_H_

#include "definitions.h"
#include <avr/io.h>
#include <util/delay.h>
#include "can.h"
#include "USART.h"

typedef struct {
	uint8_t headlight_near;
	uint8_t headlight_far;
	uint8_t turn_right; // old blink_right
	uint8_t turn_left;	// old blink_left
	uint8_t angel_eyes;
	uint8_t eyebrows;
	uint8_t lights_near;	
} front_lights;

#define HEADLIGHT_POWER_LIMIT 210

/*! Initialize the front light module and set all lights OFF

*/
void front_lights_init( void);

void front_lights_headlights( int);

void front_lights_eyebrows( BOOL);

void front_lights_turn_left( BOOL);
 
void front_lights_turn_right( BOOL);

void front_lights_angel_eyes( BOOL);

/*! In order to turn the light show on send a CAN message with ID = 80 and
	data[0] = TRUE
	
*/
void light_show( CAN_packet*, unsigned char);

/*! Send a CAN message with ID = 81 and
	data[0] = TRUE
*/
void light_handler( CAN_packet*, unsigned char);


#endif /* FRONTL_LIGHTS_MODULE_H_ */