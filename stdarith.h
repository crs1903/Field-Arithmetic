#ifndef __ARITH_H
#define __ARITH_H


#include<stdio.h>
#include<stdint.h>

/*###################################################################################
#
#		converts a char which is either 0-9 or a-b to it's eqivalent hex** value 
#
#		**here the input char ch must be a char between 0-9 or a-b otherwise the 
#		  computed value will no be as expected (it will take 0 for such char).
# 
###################################################################################*/
#define process(ch) (uint64_t)(((uint64_t)(ch)-((uint64_t)(-('0'<=(ch)&&'9'>=(ch)))&(uint64_t)'0')-((uint64_t)(-('a'<=(ch)&&'f'>=(ch)))&(uint64_t)('a'-10)))&(uint64_t)(-(('0'<=(ch)&&'9'>=(ch))||('a'<=(ch)&&'f'>=(ch)))))
#define carry(num) (((num)>>28)&1)

struct  __bigint
{
	uint64_t segment[5];  // 28 bits for data in each segment.
};

typedef struct __bigint large[1];

void 
loadnum(large des, large sor);
void
paddloadnum(large des, large sor, uint64_t padd);
void
setzero(large num);
void
read_str(large des, const char * str, unsigned int len);
void
print_str(large num);
void
print_str2(large high, large low);
uint64_t
addnum(large des, large op1, large op2);
uint64_t
subnum(large des, large op1, large op2);
void
compact(large hi, large low);
void
multnum(large high, large low, large op1, large op2);
void
num_to_int(uint64_t * hi, uint64_t * low, large op);
int64_t
msb(large num);

#endif
