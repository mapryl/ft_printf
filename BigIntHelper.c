#include <stdint.h>
#include "BigInt.h"
#include "Utils.h"

int bigIntDecimalSize(const BigInt* bi)
{
	return intDecimalSize(bi->digits[bi->size - 1]) + 9 * (bi->size - 1);
}

void bigIntFromInt(BigInt* a, int64_t b)
{
	int digit;
	bigIntCreate(a);

	digit = b % BIGINT_BASE;
	bigIntPushBack(a, digit);

	while (b /= BIGINT_BASE)
	{
		digit = b % BIGINT_BASE;
		bigIntPushBack(a, digit);
	}
}