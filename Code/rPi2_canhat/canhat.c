/*
 * CanHat 
 *
 * Created 06/05/2016
 * Author: Mark Hocevar
 * DNV GL Fuel Fighter 2016
 **
 *
 */
#include "canhat.h"


void init(void){
	printf("Init complete\n\r");
	//Serial comunication with computer init
	USART_init(MYUBRR, true);
	//Can setup
	can_init(); 
	prepare_rx(0, 1, 0, can_recv); //Read all messages
	DDRE |= (1 << DDE2) | (1<<DDE3);
	//Timer for sending can
	sei();
}

/* Function called on can receive package  */
void can_recv(CAN_packet *p, unsigned char mob){
	//Parse data from message
	//TODO do some checking of data so we don't get stupid stuff

	PORTE ^= (1 << PE3); //Green
	printf("Id: %d", p->id);
	for(int i = 0; i < p->length; i++){
		printf("%d", p->data[i]);
	}	
	


}



void can_send(){

}


