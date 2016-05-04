/*!
	\file general.h
	\brief general definitions
	
	a collection of useful macros and definitions.
	
	\author Dr. Klaus Schaefer \n
	Hochschule Darmstadt * University of Applied Sciences \n
	kschaefer@fbe.fh-darmstadt.de \n
	http://kschaefer.fbe.fh-darmstadt.de

	You can redistribute it and/or modify it 
	under the terms of the GNU General Public License.\n
	It is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; \n
	without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n
	See http://www.gnu.org/copyleft/gpl.html for mor details.
*/
#ifndef GENERAL_H
#define GENERAL_H
//! the cheapest way to define a boolean in ISO C
typedef char BOOL;
#define TRUE 1
#define FALSE 0

//8 MHz clock
#define configCPU_CLOCK_HZ 8000000l

#endif // GENERAL_H
