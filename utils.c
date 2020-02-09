#include <stdint.h>
#include "Utils.h"

size_t myMax(size_t a, size_t b)
{
	return (a > b) ? a : b;
}

int intDecimalSize(int i)
{
	int size;

	size = 1;
	while (i /= 10)
		++size;

	return size;
}

int toInt(char ch)
{
	return ch - '0';
}

int bitWidth(uint64_t bits)
{
	int result;

	result = 0;
	while (bits)
	{
		++result;
		bits <<= 1;
	}
	return result;
}

void strShiftLeft(char* str)
{
	while (*str = *(str + 1))
		++str;
}