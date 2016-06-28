
#include <stdarg.h>
#include <inttypes.h>
#include "minprintf.h"


void minprintf(char* fmt, ...)
{
	va_list 	ap;
	char 		*p, *sval;
	int 		ival, with;
	double 		dval;

	with = 0;
	va_start(ap, fmt);

	for(p = fmt; *p; p++)
	{
		if(*p != '%')
		{
			uloadChar_USB(*p);
			continue;
		}
		if(*(p+1) > '0'  && *(p+1) < '9' && (*(p+2) == 'd' || *(p+2) == 'f'))
			with = *++p - '0';
			
		switch(*++p)
		{
			case 'd':
				printInt(va_arg(ap, int), with);
				break;
			case 'f':
				printDouble(va_arg(ap, double), with);
				break;
			case 'b':
				printBinary(va_arg(ap, int));
				break;
			case 's':
				printString(va_arg(ap, char*));				
				break;
			default:
				printString("Invalid Input!");
				break;
		}
	}
	va_end(ap);
}


// Print string
void printString(char *s)
{
	while(*s)
	{
		uloadChar_USB(*s++);
	}		
		 
}


// Print 8 bit binary representation
void printBinary(int byte)
{
	int i;
	for(i = 0; i < 8; i++)
	{
		(byte & 0b10000000) ? uloadChar_USB('1') : uloadChar_USB('0');
		byte <<= 1;
	}
}

// Print int as definable length character string
void printInt(int nr, int with)
{
	char str[20];
	int i, sign;

	if((sign = nr) < 0)
		nr = -nr;
	
	i = 0;
	do
	{
		str[i++] = nr % 10 + '0';
	}
	while((nr /= 10) > 0);
	
	if(sign < 0)
		str[i++] = '-';
	str[i] = '\0';
	
	if(i < with)
	{
		for( ; with >= 0; with--)
		{
			if(with > i)
				uloadChar_USB(' ');
			else
				uloadChar_USB(str[with]);
		}
	}
	else
	{
		for( ; i >= 0; i--)
			uloadChar_USB(str[i]);
	}	
}


// Print double as definable length character string
void printDouble(double nr, int with)
{
	char str[20];
	int i, sign, i_part;

	i_part = nr*100;

	if((sign = i_part) < 0)
		i_part = -i_part;

		i = 0;
	do
	{
		str[i++] = i_part % 10 + '0';
	}
	while((i_part /= 10) > 0);
	
	if(sign < 0)
		str[i++] = '-';
	str[i] = '\0';

	if((i-2) < with)
	{
		for(with += 2 ; with >= 0; with--)
		{
			if(with > i)
				uloadChar_USB(' ');
			else if(with == 2)
			{
				uloadChar_USB(str[with]);
				uloadChar_USB('.');
			}
			else
				uloadChar_USB(str[with]);
		}
	}
	else
	{
		for( ; i >= 0; i--)
			if(i == 2)
			{
				uloadChar_USB(str[i]);
				uloadChar_USB('.');
			}
			else
				uloadChar_USB(str[i]);
	}	
}





