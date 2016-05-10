/*
 * definitions.h
 *
 * Created: 02.07.2013 17:51:58
 *  Author: Vebjørn
 * Edited: Vilius 
 * Edited: 03.21.2016 15:00:00
 */ 

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#define F_CPU 8000000

#define BAUD 9600
#define MYUBRR (F_CPU/16/BAUD)-1

#include <stdio.h>

typedef char BOOL;
#define TRUE 1
#define true 1
#define FALSE 0
#define false 0

// Used in front_light_handler()
#define HEADLIGHTS_NEAR			0b1
#define HEADLIGHTS_FAR			0b10
#define INDICATOR_RIGHT			0b100
#define INDICATOR_LEFT			0b1000
#define ANGEL_EYES				0b10000
#define EYEBROWS				0b100000

//sets bit "bit" in register "reg"
#define set_bit(reg, bit)		(reg |= (1<<bit))
//same as set_bit only clears all other bits
#define set_bit_ex(reg, bit)	(reg = (1<<bit))
//clears bit "bit" in register "reg"
#define clear_bit(reg, bit)		(reg &=~ (1<<bit))
//tests if bit "bit" is set in register "reg"
#define test_bit(reg, bit)		(reg & (1<<bit))
//toggles bit "bit" in register "reg"
#define toggle_bit(reg, bit)	(reg ^= (1<<bit))

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