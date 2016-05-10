/*
 * twi.c
 *
 * Created: 4/16/2016 11:01:02 AM
 *  Author: Vilius
 */ 

#include "twi.h"

// void twi_init( void) {
// 	//set SCL to 400kHz
// 	TWSR = 0x00;
// 	TWBR = 0x0C;
// 	//enable TWI
// 	TWCR = (1<<TWEN);
// }
// 
// void twi_start( void) {
// 	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
// 	while (test_bit(TWCR, TWINT) == 0);
// }
// 
// void twi_stop( void) {
// 	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
// }
// 
// void twi_write(uint8_t u8data) {
// 	TWDR = u8data;
// 	TWCR = (1<<TWINT)|(1<<TWEN);
// 	while(test_bit(TWCR, TWINT) == 0);
// }
// 
// uint8_t twi_read_ack( void) {
// 	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
// 	while(test_bit(TWCR,TWINT) == 0);
// 	return TWDR;
// }
// 
// uint8_t twi_get_status( void) {
// 	uint8_t status;
// 	//mask status
// 	status = TWCR & 0xF8;
// 	return status;
// }
// 
