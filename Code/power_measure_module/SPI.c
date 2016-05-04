/*
 * SPI.c
 *
 * Created: 04.07.2013 13:28:40
 *  Author: Ole
 */ 

#include "definitions.h"
#include "SPI.h"
	
void SPI_MasterInit()
{
	/* Set MOSI, SCK and SS output */
	DDR_SPI |= (1<<DD_MOSI) | (1<<DD_SCK) | (1<<DD_SS);
	DDR_SPI &= ~(1<<DD_MISO);
	
	PORT_SPI |= (1<<DD_SS);
	/* Enable SPI, Master, set clock rate fck/4 (fastest) */
	SPCR = (1<<SPE) | (1<<MSTR);
	//SPSR |= (1<<SPI2X);		//spi 2x mode
}

uint8_t SPI_MasterTransmit(uint8_t data)
{
	PORT_SPI &= ~(1<<DD_SS);
	/* Start transmission */
	SPDR = data;	
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	PORT_SPI |= (1<<DD_SS);
	//return what comes back
	return SPDR;
}

uint8_t SPI_MasterRecieve(){
	return SPI_MasterTransmit(0);
}

