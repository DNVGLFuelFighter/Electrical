/*
 * SPI.h
 *
 * Created: 04.07.2013 13:28:51
 *  Author: Ole
 */ 


#ifndef SPI_H_
#define SPI_H_

//SPI definitions
#define DDR_SPI DDRB
#define PORT_SPI PORTB
#define DD_SS PB0
#define DD_SCK PB1
#define DD_MOSI PB2
#define DD_MISO PB3

#define SPI_fastClock() SPCR = 0x50;
#define SPI_slowClock() SPCR = 0x52;

void SPI_MasterInit();
uint8_t SPI_MasterTransmit(uint8_t data);
uint8_t SPI_MasterRecieve();



#endif /* SPI_H_ */