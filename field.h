#ifndef __FIELD_H
#define __FIELD_H

#include<stdio.h>
#include<stdint.h>

#include"stdarith.h"

#define PRIME    "7fffffffffffffffffffffffffffffff"  /* the prime used here is 2^127-1*/
#define EXPONENT "7ffffffffffffffffffffffffffffffd"  /* exponent to find the inverse which is 2^127-3 */
uint64_t
isequal(large op1, large op2);
void
faddnum(large des, large op1, large op2);
void
fsubnum(large des, large op1, large op2);
void
fmultnum(large des, large op1, large op2);
void
fpow(large des, large op, large pow);
void
finvnum(large des, large op);
#endif
