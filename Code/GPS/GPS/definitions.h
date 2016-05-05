/*
 * definitions.h
 *
 * Created: 02.07.2013 17:51:58
 *  Author: Ole
 */ 


#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#define F_CPU 8000000L

#define BAUD1 38400
#define BAUD2 4800
#define MYUBRR0 F_CPU/16/BAUD1-1
#define MYUBRR1 F_CPU/16/BAUD2-1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdlib.h>
#include <avr/pgmspace.h>

typedef char BOOL;
#define TRUE 1
#define FALSE 0

//8 MHz clock
#define configCPU_CLOCK_HZ 8000000l

//bit manipulation functions
#define set_bit( reg, bit ) (reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define toggle_bit( reg, bit ) (reg ^= (1 << bit))
#define test_bit( reg , bit ) (reg & (1 << bit))

//for printing out binary
#define BYTETOBINARYPATTERN "%d%d%d%d%d%d%d%d"
#define BYTETOBINARY(byte)  \
(byte & 0x80 ? 1 : 0), \
(byte & 0x40 ? 1 : 0), \
(byte & 0x20 ? 1 : 0), \
(byte & 0x10 ? 1 : 0), \
(byte & 0x08 ? 1 : 0), \
(byte & 0x04 ? 1 : 0), \
(byte & 0x02 ? 1 : 0), \
(byte & 0x01 ? 1 : 0)

#endif /* DEFINITIONS_H_ */