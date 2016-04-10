/*
 * spic.c
 *
 * Created: 3/31/2016 10:20:30 AM
 *  Author: Vilius
 */ 

#include "spi.h"

void spi_init_master( void){
	/* Set MOSI and SCK output, all others input */
	//DDRB = (1<<DDB3)|(1<<DDB1);
	DDRB |= (1<<DDB3)|(1<<DDB2)|(1<<DDB1)|(1<<DDB0);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	
	DAC_DESELECT;
}

void spi_init_slave( void) {
	/* Set MISO output, all others input */
	DDRB = (1<<DDB3);
	/* Enable SPI */
	SPCR = (1<<SPE);
}

void spi_master_tx(unsigned char c) {
	/* Start transmission */
	SPDR = c;
	/* Wait for transmission complete */
	while(!(test_bit(SPSR, SPIF)));
}

unsigned char spi_slave_rx( void) {
	/* Wait for reception complete */
	while(!(test_bit(SPSR, SPIF)));
	/* Return data register */
	return SPDR;
}
