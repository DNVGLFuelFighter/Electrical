/*
 * spic.c
 *
 * Created: 3/31/2016 10:20:30 AM
 *  Author: Vilius
 */ 

#include "spi.h"



void spi_init_master( void){
	/* Set MOSI and SCK output, all others input */
	//DDRB = (1<<DDB2)|(1<<DDB1);

	DDRB |= (1<<DDB2)|(1<<DDB1)|(1<<DDB0);
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

unsigned char spi_master_tx(unsigned char c) {
	/* Select slave */
	DAC_SELECT;
	/* Start transmission */
	SPDR = c;
	/* Wait for transmission complete */
	while(!(test_bit(SPSR, SPIF)));
	/* De-select slave */
	DAC_DESELECT;
	/* Return received data */
	return SPDR;
}

unsigned char spi_slave_rx( unsigned char data) {
	SPDR = data;
	/* Wait for reception complete */
	while(!(test_bit(SPSR, SPIF)));
	/* Return data register */
	return SPDR;
}

unsigned char spi_master_tx_str(unsigned char s[]) {
	int i = 0;
	
	while(s[i] != NULL) {
		spi_master_tx(s[i]);
		i++;
	}
	return SPDR;
}
