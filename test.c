#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include"field.h"

int main()
{
	char a[33], b[33];
	large num1, num2, hi, low, sum, sub, fsum, fmult, finv, fmult2;
	printf("make sure the Entered number is 127 bit otherwise\nit will only take last 127 bits\n\n");
	printf("Enter the first number a in hex:\n");
	scanf("%32s", a);
	printf("Enter the second number b in hex:\n");
	scanf("%32s", b);
	
	read_str(num1, a, strlen(a));
	read_str(num2, b, strlen(b));
	
	addnum(sum, num1, num2);
	multnum(hi, low, num1, num2);
	subnum(sub, num2, num1);
	faddnum(fsum, num1, num2);
	finvnum(finv, num1);
	fmultnum(fmult, num1, finv);
	fmultnum(fmult2, num1, num2);
	
	printf("\n\nMultiplication a * b:\n");
	print_str2(hi, low);
	printf("\n\nAddition a + b:\n");
	print_str(sum);
	printf("\n\nSubstraction b-a:\n");
	print_str(sub);
	printf("\n\nField inv of a :\n");
	print_str(finv);
	printf("\n\nField Multiplication a * inv(a):\n");
	print_str(fmult);
	printf("\n\nField Addition a + b:\n");
	print_str(fsum);
	printf("\n\nField Multiplication a * b:\n");
	print_str(fmult2);
	
}
