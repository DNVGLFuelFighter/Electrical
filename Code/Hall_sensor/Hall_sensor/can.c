/*!	\file
	generic CAN bus driver for AT90CAN128

	privides interrupt-driven reception and polled transmission of can packets

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

#include "avr/interrupt.h"
#include "definitions.h"
#include "assert.h"
#include "can.h"

CAN_cbf canlist[NO_MOBS]; //!< can callback list for RX channels, indexed by MOB number

//! CAN interrupt service routine

//! presently only used for reception of CAN packets
ISR( CANIT_vect)
	{
	unsigned i;
	static CAN_packet packet;
	char save_canpage=CANPAGE;
	
	unsigned mob=CANHPMOB; // get highest prio mob
    CANPAGE = mob & 0xf0;
	mob >>= 4; // -> mob number 0..15
	ASSERT( (CANSTMOB & ~0xa0) ==0); // allow only RX ready and DLC warning
	register char length;
	packet.id=(CANIDT2>>5) | (CANIDT1 <<3);
	length=CANCDMOB & 0x0f;
	packet.length=length;
	for (i = 0; i <length; ++i)
		{
		 packet.data[i] = CANMSG;
		}
	CANSTMOB=0;		// reset INT reason
	CANCDMOB=0x80;	// re-enable RX on this channel
	canlist[ mob]( &packet, mob);
	CANPAGE=save_canpage;	// restore CANPAGE
	}

BOOL can_tx( char mob, 	CAN_packet *packet)
	{
	unsigned cnt;
	ASSERT( packet->id <= 0x7ff);
	ASSERT( packet->length <=8);
	ASSERT( mob <=14);
    CANPAGE = mob << 4;
	if(
		(  CANCDMOB & 0b11000000) // if MOB in use
		&& 
	  	( (CANSTMOB & 0x40) ==0)   // TX not ready
	  )
		return FALSE;
	CANSTMOB = 0x00;    	// cancel pending operation 
	CANCDMOB = 0x00;		
	if( packet->length!=RTR)
		{
		CANIDT1=packet->id >>3;
		CANIDT2=packet->id <<5;
        CANIDT3 = 0;
        CANIDT4 = 0;
		for (cnt=0; cnt < packet->length; ++cnt) 
			CANMSG = packet->data[cnt];         
    	CANCDMOB|=packet->length;
		}
	else
		{
		CANIDT1=packet->id >>3;
		CANIDT2=packet->id <<5;
        CANIDT3 = 0;
        CANIDT4 = 1<<RTRTAG;
		CANCDMOB|=packet->length;
		}
    CANCDMOB|=0x40; //enable TX
	return TRUE;
	}

BOOL prepare_rx( char mob, unsigned id, unsigned idmask, CAN_cbf callback)
	{
	if( mob >= 15) 				// illegal MOB number
		return TRUE;
	canlist[ (unsigned)mob]=callback;
    CANPAGE = mob << 4;
	if( callback==0)			// release this MOB 
		{
		CANSTMOB = 0x00;    	// cancel pending operation
		CANCDMOB = 0x00;
		unsigned mask=1<<mob;
		CANIE2 &=  ~mask;
		CANIE1 &= ~(mask>>8);
		return FALSE;
		}
	if( CANCDMOB & 0b11000000) 	// if MOB already in use
		return TRUE;			// no vacancy ...
	CANSTMOB = 0x00;    	// cancel pending operation 
	CANCDMOB = 0x00;		
	CANHPMOB = 0x00;		// enable direct mob indexing, see docu
	CANIDT1=id >>3;
	CANIDT2=id <<5;
	CANIDM1=idmask >>3;
	CANIDM2=idmask <<5;
	CANIDM3=0;
	CANIDM4=0;
	CANCDMOB=0x80; // enable RX
	unsigned mask=1<<mob;
	CANIE2 |=  mask;
	CANIE1 |= (mask>>8);
	return FALSE;
	}


void can_init( void)
	{
	unsigned mob;
//! \bug different baudrate support not implemented yet 
#if configCPU_CLOCK_HZ==8000000l
	CANBT1=0x06;	// 8MHz, 100kbit/s
	CANBT2=0x0c;
	CANBT3=0x5a;
#else
	CANBT1=0x26;	// 16 MHz, 100kbit/s
	CANBT2=0x04;
	CANBT3=0x13;
#endif
	
	for (mob = 0; mob < NO_MOBS; mob++)
		{
		CANPAGE  = (mob << 4);
		CANSTMOB = 0;
		CANCDMOB = 0;
		}
	CANGCON |= 0x02;
	CANGIE=(1<<ENIT) | (1<< ENRX);
	}

