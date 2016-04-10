/*
 * spi.h
 *
 * Created: 3/31/2016 10:20:37 AM
 *  Author: Vilius
 */ 


#ifndef SPI_H_
#define SPI_H_

#include "definitions.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

#define DAC_SELECT		clear_bit(PORTB, PB0)
#define DAC_DESELECT	set_bit(PORTB, PB0);

void spi_init_master( void);
void spi_init_slave( void);
void spi_master_tx(unsigned char c);
unsigned char spi_slave_rx( void);



#endif /* SPI_H_ */