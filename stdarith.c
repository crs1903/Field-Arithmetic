#include<stdio.h>
#include<stdint.h>

#include "stdarith.h"

/*###################################################################################
#
#				This file contains all the function defintions we
#				need for big integer arithmetic. operations like 
#				addition, substraction, multiplication for big integers 
#				(127 bits max)
#				
#				Other functions like printing, loading, reading/ scaning etc.
#				are also defined here.
#
#----------------------------------------------------------------------------------*/

/*###################################################################################
#
#			copying one 127 bit integer into another
#
#----------------------------------------------------------------------------------*/

void
loadnum(large des, large src)
{
	des->segment[0] = src->segment[0];
	des->segment[1] = src->segment[1];
	des->segment[2] = src->segment[2];
	des->segment[3] = src->segment[3];
	des->segment[4] = src->segment[4];
}

/*###################################################################################
#
#			copying one 127 bit integer into another
#			by padding** (using bitwise and) an 64 bit integer 
#			to each of it's segment
#
#			**this will be useful to avoid if-else statements
#
#----------------------------------------------------------------------------------*/

void
paddloadnum(large des, large src, uint64_t padd)
{
	des->segment[0] = src->segment[0]&padd;
	des->segment[1] = src->segment[1]&padd;
	des->segment[2] = src->segment[2]&padd;
	des->segment[3] = src->segment[3]&padd;
	des->segment[4] = src->segment[4]&padd;
}

/*###################################################################################
#
#			setting the structure to store the value 0
#
#----------------------------------------------------------------------------------*/

void
setzero(large a)
{
	a->segment[0] = 0UL;
	a->segment[1] = 0UL;
	a->segment[2] = 0UL;
	a->segment[3] = 0UL;
	a->segment[4] = 0UL;
}

/*###################################################################################
#
#			reading a number from a given hexadecimal string of char
#			and putting it in the destination
#
#----------------------------------------------------------------------------------*/

void
read_str(large des, const char * str, unsigned int len)
{
	setzero(des);
	uint64_t low, high, c;
	int offset = 0;

	/* loading last 16 hex digits into the 64 bit integer low */

	c = 0;
	for(int i = 0; i < 16 && i < len ; i++)
	{
		c |= process(str[len-1-i])<<(offset);
		offset=(offset+4)&63;
	}
	low = c;

	/* loading next 16 hex digits into the 64 bit integer high */

	c = 0;
	for(int i = 16; i < 32 && i < len ; i++)
	{
		c |= process(str[len-1-i])<<(offset);
		offset=(offset+4)&63;
	}
	high = c;

	/* setting each segment of destination using high and low */

	des->segment[0] = low & 0xfffffff;
	low>>=28;
	des->segment[1] = low & 0xfffffff;
	low>>=28;
	des->segment[2] = (low & 0xff)|((high & 0xfffff)<<8);
	high>>=20;
	des->segment[3] = high & 0xfffffff;
	high>>=28;
	des->segment[4] = high & 0x7fff;
}

/*###################################################################################
#
#			printing one 127 bit number
#
#----------------------------------------------------------------------------------*/

void
print_str(large des)
{
	uint64_t low, high, c;
	low = 0;
	high = 0;

	/* taking lower 64 bit into low and higher 64 bit into high and printing it */

	 low |= (des->segment[0]);
	 low |= (des->segment[1]<<28);
	 low |= (des->segment[2]<<56);
	high |= (des->segment[2]>>8);
	high |= (des->segment[3]<<20);
	high |= (des->segment[4]<<48);

	printf("%lx%.16lx\n", high, low);

}

/*###################################################################################
#
#			printing two 127 bit numbers as one 254 bit number
#
#----------------------------------------------------------------------------------*/

void
print_str2(large hi, large low)
{
	uint64_t __low, __high, _low, _high, c;
	__low = 0;
	__high = 0;
	_low = 0;
	_high = 0;

	__low |= (low->segment[0]);
	__low |= (low->segment[1]<<28);
	__low |= (low->segment[2]<<56);
	__high|= (low->segment[2]>>8);
	__high|= (low->segment[3]<<20);
	__high|= (low->segment[4]<<48);
	__high|= (hi->segment[0]<<63);
	_low  |= (hi->segment[0]>>1);
	_low  |= (hi->segment[1]<<27);
	_low  |= (hi->segment[2]<<55);
	_high |= (hi->segment[2]>>9);
	_high |= (hi->segment[3]<<19);
	_high |= (hi->segment[4]<<47);

	printf("%lx%.16lx%.16lx%.16lx\n", _high, _low, __high, __low);

}

/*###################################################################################
#
#			addition of two 127 bit numbers and
#			return the carry
#
#----------------------------------------------------------------------------------*/

uint64_t
addnum(large res, large _a, large _b)
{
	uint64_t sum, c;
	large a, b;
	loadnum(a, _a);
	loadnum(b, _b);

	setzero(res);

	sum = a->segment[0] + b->segment[0];
	c = carry(sum);
	res->segment[0] = sum & 0xfffffff;

	sum = a->segment[1] + b->segment[1] + c;
	c = carry(sum);
	res->segment[1] = sum & 0xfffffff;

	sum = a->segment[2] + b->segment[2] + c;
	c = carry(sum);
	res->segment[2] = sum & 0xfffffff;

	sum = a->segment[3] + b->segment[3] + c;
	c = carry(sum);
	res->segment[3] = sum & 0xfffffff;

	sum = a->segment[4] + b->segment[4] + c;
	c = (sum>>15)&1;
	res->segment[4] = sum & 0x7fff;

	return c;
}

/*###################################################################################
#
#			substraction of two 127 bit numbers and
#			return the carry
#
#----------------------------------------------------------------------------------*/

uint64_t
subnum(large des, large _a, large _b)
{
	large one, b;
	loadnum(b, _b);
	setzero(one);
	one->segment[0] = 1UL;
	b->segment[0] = (b->segment[0] ^ 0xfffffffUL);
	b->segment[1] = (b->segment[1] ^ 0xfffffffUL);
	b->segment[2] = (b->segment[2] ^ 0xfffffffUL);
	b->segment[3] = (b->segment[3] ^ 0xfffffffUL);
	b->segment[4] = (b->segment[4] ^ 0x7fffUL);
	
	addnum(b, b, one);
	return addnum(des, _a, b);
}
/*###################################################################################
#
#			puting high 127 bits in 'high' and low 127 bits in 'low' 
#
#----------------------------------------------------------------------------------*/
void
compact(large hi, large low)
{
	/* puting high 127 bits in 'high' and low 127 bits in 'low' */

	 hi->segment[4] = ((hi->segment[4]<<13)|(hi->segment[3]>>15))&0xfffffff;
	 hi->segment[3] = ((hi->segment[3]<<13)|(hi->segment[2]>>15))&0xfffffff;
	 hi->segment[2] = ((hi->segment[2]<<13)|(hi->segment[1]>>15))&0xfffffff;
	 hi->segment[1] = ((hi->segment[1]<<13)|(hi->segment[0]>>15))&0xfffffff;
	 hi->segment[0] = ((hi->segment[0]<<13)|(low->segment[4]>>15))&0xfffffff;
	 low->segment[4] &= 0x7fff;
}

/*###################################################################################
#
#			multiplication of two 127 bit numbers and
#			put the high 127 bits and low 127 bits in two different variables
#
#----------------------------------------------------------------------------------*/

void
multnum(large hi, large low, large _a, large _b)
{
	large a, b;
	loadnum(a, _a);
	loadnum(b, _b);

	setzero(hi);
	setzero(low);

	/* using school-book multiplication here */

	low->segment[0] = (a->segment[0])*(b->segment[0]);
	low->segment[1] = (a->segment[0])*(b->segment[1]) + (a->segment[1])*(b->segment[0]);
	low->segment[2] = (a->segment[0])*(b->segment[2]) + (a->segment[2])*(b->segment[0]) + (a->segment[1])*(b->segment[1]);
	low->segment[3] = (a->segment[0])*(b->segment[3]) + (a->segment[1])*(b->segment[2]) + (a->segment[2])*(b->segment[1]) + (a->segment[3])*(b->segment[0]);
	low->segment[4] = (a->segment[0])*(b->segment[4]) + (a->segment[1])*(b->segment[3]) + (a->segment[2])*(b->segment[2]) + (a->segment[3])*(b->segment[1]) + (a->segment[4])*(b->segment[0]);
	 hi->segment[0] = (a->segment[1])*(b->segment[4]) + (a->segment[2])*(b->segment[3]) + (a->segment[3])*(b->segment[2]) + (a->segment[4])*(b->segment[1]);
	 hi->segment[1] = (a->segment[2])*(b->segment[4]) + (a->segment[3])*(b->segment[3]) + (a->segment[4])*(b->segment[2]);
	 hi->segment[2] = (a->segment[3])*(b->segment[4]) + (a->segment[4])*(b->segment[3]);
	 hi->segment[3] = (a->segment[4])*(b->segment[4]);

	/* making each segment of both high and low have only 28 bits for data */

	low->segment[1] += (low->segment[0]>>28);
	low->segment[2] += (low->segment[1]>>28);
	low->segment[3] += (low->segment[2]>>28);
	low->segment[4] += (low->segment[3]>>28);
	 hi->segment[0] += (low->segment[4]>>28);
	 hi->segment[1] +=  (hi->segment[0]>>28);
	 hi->segment[2] +=  (hi->segment[1]>>28);
	 hi->segment[3] +=  (hi->segment[2]>>28);
	 hi->segment[4] +=  (hi->segment[3]>>28);

	/* Dropping excess bits */

	low->segment[0] &= 0xfffffff;
	low->segment[1] &= 0xfffffff;
	low->segment[2] &= 0xfffffff;
	low->segment[3] &= 0xfffffff;
	low->segment[4] &= 0xfffffff;
	 hi->segment[0] &= 0xfffffff;
	 hi->segment[1] &= 0xfffffff;
	 hi->segment[2] &= 0xfffffff;
	 hi->segment[3] &= 0xfffffff;
	 hi->segment[4] &= 0x3;

	 compact(hi, low);

}

/*###################################################################################
#
#			putting a 127 bit number in two 64 bit integers
#
#----------------------------------------------------------------------------------*/

void
num_to_int(uint64_t * _hi, uint64_t * _low, large src)
{
	uint64_t low, high, c;
	low = 0;
	high = 0;

	/* taking lower 64 bit into low and higher 64 bit into high */

	 low |= (src->segment[0]);
	 low |= (src->segment[1]<<28);
	 low |= (src->segment[2]<<56);
	high |= (src->segment[2]>>8);
	high |= (src->segment[3]<<20);
	high |= (src->segment[4]<<48);
	
	*_hi = high;
	*_low = low;
}

/*###################################################################################
#
#			Returns most significant bit position of a 
#			127 bit integer. Returns -1 if number is 0
#
#----------------------------------------------------------------------------------*/

int64_t
msb(large num)
{
	int64_t t=0, flag=0, j, i=0, k;
	for(j=4; j>=0; j--)
	{
		i |= (-(num->segment[j]!=0)) & (flag-1) & j;
		flag |= (num->segment[j]!=0);
	}
	
	/*	This calculates as i = i*28
	    since, 28 = (2^4)+(2^3)+(2^2) */
						    
	t += (i<<4)+(i<<3)+(i<<2); 		
	
	uint64_t hi, low, var, a=num->segment[i];
	for(k = 32; k!=0; k>>=1)
	{
		hi = a>>k;
		low = a&((1UL<<k)-1UL);
		var = (hi>0);
		t += (-var) & k;
		a = ((-var) & hi) | ((var-1) & low);
	}
	t = ((-flag)&t)|(flag-1);
	return t;
}

