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
#include <avr/interrupt.h>
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

#define HEADLIGHT_POWER_LIMIT	150

extern BOOL EMERG;
extern BOOL IND_LEFT;
extern BOOL IND_RIGHT;

/*! Initialize the front light module and set all lights OFF
*/
void front_lights_init( void);

/*!	Turn on both headlights
	\param power. Range 0..210
	NB! headlights are connected in series
*/
void front_lights_headlights( int power);

/*! Turn on both lights called eyebrows (same LED as indicator)
	\param on TRUE to turn on, FALSE to turn off
*/
void front_lights_eyebrows( BOOL on);

/*! Turn on the left indicator light
	\param on TRUE to turn on, FALSE to turn off
*/
void front_lights_turn_left( BOOL on);
 
/*! Turn on the right indicator light
	\param on TRUE to turn on, FALSE to turn off
*/
void front_lights_turn_right( BOOL on);

/*! Turn on the lights called angel eyes
	\param on TRUE to turn on, FALSE to turn off
*/
void front_lights_angel_eyes( BOOL on);

/*! Flashy light show
*/
void light_show();

/*! Main handler of all the front lights
	\param p CAN packet
	\param mob message object. Range 0..14
	See "can.h" for further explanation on params
	To update lights send a CAN message data.ID = ID_lightsFront,
	data.length = 1, data[0] = range 0bx..0bxxxxxx
*/
void front_light_handler( CAN_packet*, unsigned char);

/*! Turn on left indicator
	\param on TRUE to turn on, FALSE to turn off
*/
void front_ind_left( BOOL on);

/*! Turn on right indicator
	\param on TRUE to turn on, FALSE to turn off
*/
void front_ind_right( BOOL on);

/*! Turn on the emergency ind lights 
	\param on TRUE to turn on, FALSE to turn off
*/
BOOL get_ind_left( void);
BOOL get_ind_right( void);

void front_toggle_ind_left( void);
void front_toggle_ind_right( void);

void front_emergency( BOOL on);



#endif /* FRONTL_LIGHTS_MODULE_H_ */