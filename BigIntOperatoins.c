#include <stdlib.h>
#include "BigInt.h"

void bigIntCreate(BigInt* bigInt)
{
	bigInt->size = 0;
}

void bigIntCreateSize(BigInt* bigInt, int size)
{
	bigInt->size = size;
	while (--size >= 0)
		bigInt->digits[size] = 0;
}

int bigIntBack(const BigInt* bigInt)
{
	return bigInt->digits[bigInt->size - 1];
}

void bigIntPushBack(BigInt* bigInt, int val)
{
	if (bigInt->size == BIGINT_SIZE)
	{
		printf("bigInt overflow"); //xxxx write
		exit(1);
	}
	else
		bigInt->digits[bigInt->size++] = val;
}

void bigIntPopBack(BigInt* bigInt)
{
	if (bigInt->size == 0)
	{
		printf("bigInt underflow"); //xxxx write
		exit(1);
	}
	else
		--bigInt->size;
}
