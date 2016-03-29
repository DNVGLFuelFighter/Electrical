/*!
	\author Dr. Klaus Schaefer \n
	Hochschule Darmstadt * University of Applied Sciences \n
	schaefer@eit.h-da.de \n
	http://kschaefer.eit.h-da.de


	You can redistribute it and/or modify it 
	under the terms of the GNU General Public License.\n
	It is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; \n
	without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n
	See http://www.gnu.org/copyleft/gpl.html for mor details.
*/
#ifndef _CAN_H__
#define _CAN_H__

#include "definitions.h"

#define ID_brakes			1
#define ID_H2				6
#define ID_motorOnOff		7
#define ID_ESOutside		8
#define ID_ESInside			9
#define ID_SDRelay			11

/* TODO
Various sensors via ADC */
//#define ID_ADCsensors		12
#define ID_motorCtrlRight	21
#define ID_motorCtrlLeft	22
#define ID_hallSensor		23
#define ID_steeringWheel	31
#define ID_buttons			32
#define ID_screen			41
#define ID_backCamera		42
#define ID_GPS				51
#define ID_IMU				52
#define ID_SD				53
#define ID_3G				61
#define ID_horns			71
#define ID_fans				72
#define ID_lightShow		80
#define ID_lightsFront		81
#define ID_lightsBreak		91
#define ID_backSigRight		92
#define ID_backSigLeft		93


#define NO_MOBS 15 		//!< number of message object buffers (MOBs) (AT90CAN128)
#define NOMOB 	0xff 	//!< return value for getmob()
#define RTR		0xff 	//!< special value for length ->RTR packet

//! can packet structure

//! used to describe a CAN20a (standard 11 bit) packet.
//! a packet length of 0 indicates an RTR packet
typedef struct
	{
	unsigned id;			//!< CAN identifier
	unsigned char length;	//!< CAN data length
	unsigned char data[8];	//!< CAN data array [8]
	}
	CAN_packet;

//! CAN callback function for RX

//! signature of functions that will be called 
//! on a CAN receiver interrupt if the corresponding id/mask pattern was enabled.
typedef void (* CAN_cbf)( CAN_packet *p, unsigned char mob);

/*! can receive routine
	\param mob message object buffer to use (1..14)
	\param id CAN identifier / id range
	\param idmask mask telling which bits in id to check
	\param callback pointer to callback function (isr callback)
	\return FALSE if MOB not ready, TRUE otherwise
*/
BOOL prepare_rx( char mob, unsigned id, unsigned idmask, CAN_cbf callback);

/*! can transmit routine
	\param mob MOB channel to use. 
	The receive function uses low MOB numbers 0,1, ...
	Therefore use high numbers 15, 14 ... for transmission.
	Ideally, use different MOBs for different IDs to maintain priority.
	\param packet pointer to CAN packet
	\return FALSE if TX on this MOB not ready, TRUE otherwise
*/
BOOL can_tx( char mob, 	CAN_packet *packet);

/*! can interface initialization

	call this function before any CAN related function is used
*/
void can_init( void);


/*! Displays current can channel information via printf foo

*/
void can_health(void);

/*! Send a complete CAN packet over the can bus
	For parameter description see "can.h"
*/
BOOL can_packet_send( char mob, CAN_packet* packet);

#endif
