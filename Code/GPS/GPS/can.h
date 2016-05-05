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
#include "definitions.h"

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

/*! can interface initialisation

	call this function before any CAN related function is used
*/
void can_init( void);
