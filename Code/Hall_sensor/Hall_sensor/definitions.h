/*
 * definitions.h
 *
 * Created: 02.07.2013 17:51:58
 *  Author: Vebjørn
 */ 


#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#define F_CPU 8000000UL

#define BAUD 9600
/* Asynchronous Normal mode(U2Xn = 0) */
#define MYUBRR F_CPU/16/BAUD-1

#include <stdio.h>

typedef char BOOL;
#define TRUE 1
#define FALSE 0

//8 MHz clock
#define configCPU_CLOCK_HZ 8000000l

//bit manipulation functions
#define set_bit( reg, bit ) (reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define toggle_bit( reg, bit ) (reg ^= (1 << bit))
#define get_bit( reg , bit ) (reg & (1 << bit))




//for printing out binary
/*
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
*/

#ifndef ASSERT
//! ASSERT macro for debugging. ATMEL specific.
#ifndef RELEASE
#define ASSERT(x) if(!(x)) asm("break")
#else
#define ASSERT(x) if(!(x)) asm("jmp 0")
#endif
#endif

#endif /* DEFINITIONS_H_ */