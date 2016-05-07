/*
 * Canhat. Reads all can packages and send them trough serial to raspberry Pi.
 *
 * Created: 06/05/2016 
 * Author : Mark Hocevar
 */ 
#include "canhat.h"
int main(void){
	init();
	DDRE |= (1 << DDE2) | (1<<DDE3); 
	PORTE |= (1 << PE2) | (1<<PE3);
	
	//Turn on
	PORTE &= ~(1<<PE3);
	
	while(1){
		//printf("Hello world\n");
		_delay_ms(2000);	
		PORTE ^= (1 << PE2); //Red
	}
}



