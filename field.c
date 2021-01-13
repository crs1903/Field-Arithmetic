#include"field.h"
#include<stdio.h>
#include<stdint.h>

/*###################################################################################
#
#				This file contains all the function defintions 
#				we need for field arithmetic. operations like 
#				addition, substraction, multiplication and inversion
#				all in field GF(p) for p = 2^127-1 
#
#				standard functions like addnum, subnum, multnum, setzero,
#				loadnum, paddloadnum etc. are loaded from "stdarith.h"
#				(included in "field.h")
#
#----------------------------------------------------------------------------------*/

/*###################################################################################
#
#			checking two 127 bit integers 
#			if they are equal returns 0 else returns 1		 
#
#----------------------------------------------------------------------------------*/


uint64_t
isequal(large a, large b)
{
	uint64_t c = (a->segment[0]^b->segment[0])|(a->segment[1]^b->segment[1])|(a->segment[2]^b->segment[2])|(a->segment[3]^b->segment[3])|(a->segment[4]^b->segment[4]);
	c = (c!=0UL);
	return c;
}
/*###################################################################################
#
#			adding two 127 bit integers in the field Zp
#			where p = 2^127-1  			 
#
#----------------------------------------------------------------------------------*/

void
faddnum(large res, large _a, large _b)
{
	large a, b, p, sum, CARRY ;
	loadnum(a, _a);
	loadnum(b, _b);
	read_str(p, PRIME, 32);
	setzero(CARRY);
	
	uint64_t c = addnum(sum, a, b);
	CARRY->segment[0] = c;
	addnum(sum, sum, CARRY);
	c = isequal(sum, p);
	
	paddloadnum(res, sum, -c);	/* if c is 1 then res is loaded with sum else res will be zero since sum = 0(mod p) */
}

/*###################################################################################
#
#			substracting two 127 bit integers in the field Zp
#			where p = 2^127-1  			 
#
#----------------------------------------------------------------------------------*/

void
fsubnum(large res, large _a, large _b)
{
	large a, b, p, CARRY ;
	loadnum(a, _a);
	loadnum(b, _b);
	read_str(p, PRIME, 32);
	setzero(CARRY);
	subnum(b, p ,b);
	uint64_t c = addnum(res, a, b);
	CARRY->segment[0] = c;
	addnum(res, res, CARRY);
	c = isequal(res, p);
	
	paddloadnum(res, res, -c);
}

/*###################################################################################
#
#			multiplying two 127 bit integers in the field Zp
#			where p = 2^127-1
#
#----------------------------------------------------------------------------------*/

void
fmultnum(large res, large _a, large _b)
{
	large hi, low, a, b;
	loadnum(a, _a);
	loadnum(b, _b);
	multnum(hi, low, a, b);
	faddnum(res, hi, low);
}

/*###################################################################################
#
#			calculating a 127 bit integer to the power of 
#			another 127 bit integer in the field Zp where p = 2^127-1
#
#----------------------------------------------------------------------------------*/

void
fpow(large des, large op, large pow)
{	
	
	large res, R1, R2, temp_1, temp_2;
	setzero(res);
	setzero(R1);
	setzero(R2);
	res->segment[0] = 1UL;
	uint64_t high, low, bit;
	int64_t offset, MSB;
	num_to_int(&high, &low, pow);
	
	MSB = msb(pow);
	
	/* Since the exponent has total 127 bits then then are 63 bits in high and 64 bits in low */
	
	for(offset = MSB-64; offset >= 0; offset--)
	{
		fmultnum(R1, res, res);
		fmultnum(R2, R1, op);
		bit = (high>>(offset)&1);
		paddloadnum(temp_1, R2, -bit);			/* if bit is 1 then it loads R2 to temp_1 or it loads 0 */
		paddloadnum(temp_2, R1, (bit-1UL));		/* if bit is 0 then it loads R1 to temp_2 or it loads 0 */
		faddnum(res, temp_1, temp_2);			/* res will have R1 if bit is 0 else will have R2       */
	}
	MSB = ((-(MSB < 63))&MSB) | (((MSB < 63)-1)&63);
	
	for(offset = MSB; offset >= 0; offset--)
	{
		fmultnum(R1, res, res);
		fmultnum(R2, R1, op);
		bit = (low>>(offset)&1);
		paddloadnum(temp_1, R2, -bit);			/* if bit is 1 then it loads R2 to temp_1 or it loads 0 */
		paddloadnum(temp_2, R1, (bit-1UL));		/* if bit is 0 then it loads R1 to temp_2 or it loads 0 */
		faddnum(res, temp_1, temp_2);			/* res will have R1 if bit is 0 else will have R2       */
	}
	loadnum(des, res);

}
/*###################################################################################
#
#			inversing a 127 bit integer in the field Zp
#			where p = 2^127-1 (similar to power with p-2 as exponent)
#
#----------------------------------------------------------------------------------*/

void
finvnum(large des, large op)
{	
	
	large EXP, res, R1, R2, temp_1, temp_2;
	setzero(res);
	setzero(R1);
	setzero(R2);
	res->segment[0] = 1UL;
	uint64_t high, low, bit;
	int offset;
	read_str(EXP, EXPONENT, 32);
	num_to_int(&high, &low, EXP);
	
	/* Since the exponent has total 127 bits then then are 63 bits in high and 64 bits in low */
	
	for(offset = 62; offset >= 0; offset--)
	{
		fmultnum(R1, res, res);
		fmultnum(R2, R1, op);
		bit = (high>>(offset)&1);
		paddloadnum(temp_1, R2, -bit);			/* if bit is 1 then it loads R2 to temp_1 or it loads 0 */
		paddloadnum(temp_2, R1, (bit-1UL));		/* if bit is 0 then it loads R1 to temp_2 or it loads 0 */
		faddnum(res, temp_1, temp_2);			/* res will have R1 if bit is 0 else will have R2       */
	}
	for(offset = 63; offset >= 0; offset--)
	{
		fmultnum(R1, res, res);
		fmultnum(R2, R1, op);
		bit = (low>>(offset)&1);
		paddloadnum(temp_1, R2, -bit);			/* if bit is 1 then it loads R2 to temp_1 or it loads 0 */
		paddloadnum(temp_2, R1, (bit-1UL));		/* if bit is 0 then it loads R1 to temp_2 or it loads 0 */
		faddnum(res, temp_1, temp_2);			/* res will have R1 if bit is 0 else will have R2       */
	}
	loadnum(des, res);

}

