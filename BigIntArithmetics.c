#include <stdlib.h>

#include "BigInt.h"
#include "Utils.h"

void bigIntAdd(BigInt* a, const BigInt* b)
{
	int carry;
	size_t i;

	carry = 0;
	i = 0;
	while (i < myMax(a->size, b->size) || carry)
	{
		if (i == a->size)
			bigIntPushBack(a, 0);
		a->digits[i] += carry + (i < b->size ? b->digits[i] : 0);
		carry = a->digits[i] >= BIGINT_BASE;
		if (carry)
			a->digits[i] -= BIGINT_BASE;
		++i;
	}
}

void bigIntMultiply(BigInt* a, int b)
{
	int carry;
	size_t i;

	carry = 0;
	i = 0;
	while (i < a->size || carry)
	{
		if (i == a->size)
			bigIntPushBack(a, 0);
		long long cur = carry + a->digits[i] * 1ll * b;
		a->digits[i] = (int)(cur % BIGINT_BASE);
		carry = (int)(cur / BIGINT_BASE);
		++i;
	}
	while (a->size > 1 && (bigIntBack(a) == 0))
		bigIntPopBack(a);
}

void bigIntMultiplyBig(BigInt* c, const BigInt* a, const BigInt* b)
{
	size_t i;
	size_t j;
	int carry;
	long long cur;

	bigIntCreateSize(c, a->size + b->size);
	i = 0;

	while (i < a->size)
	{
		j = 0;
		carry = 0;
		while (j < b->size || carry)
		{
			cur = c->digits[i + j] + a->digits[i] * 1ll * (j < b->size ? b->digits[j] : 0) + carry;
			c->digits[i + j] = (int)(cur % BIGINT_BASE);
			carry = (int)(cur / BIGINT_BASE);
			++j;
		}
		++i;
	}
	while (c->size > 1 && bigIntBack(c) == 0)
		bigIntPopBack(c);
}

void nPowY(BigInt* bigIntResult, int n, int y)
{
	bigIntResult->size = 0;
	bigIntPushBack(bigIntResult, n);

	while (--y > 0)
	{
		bigIntMultiply(bigIntResult, n);
	}
}

void bigIntDivide(BigInt* a, int b)
{
	int carry;
	int i;
	long long cur;

	carry = 0;
	i = (int)a->size - 1;
	while (i >= 0)
	{
		cur = a->digits[i] + carry * 1ll * BIGINT_BASE;
		a->digits[i] = (int)(cur / b);
		carry = (int)(cur % b);
		--i;
	}
	while (a->size > 1 && bigIntBack(a) == 0)
		bigIntPopBack(a);
}