#include "UnpackDouble.h"
#include "BigInt.h"
#include "Utils.h"
#include "DoubleUnpacked.h"

void CalcFractional(BigInt* fractional, uint64_t fract, int initialPow, int denormal)
{
	int width;
	BigInt pow;
	BigInt pow1;

	width = bitWidth(fract) + initialPow;
	nPowY(&pow, 10, width);
	nPowY(&pow1, 10, width);
	bigIntCreate(fractional);
	if (initialPow)
	{
		while (initialPow--)
			bigIntDivide(&pow, 2);
		if (!denormal)
			bigIntAdd(fractional, &pow);
	}
	while (fract)
	{
		bigIntDivide(&pow, 2);
		if (fract & 0x8000000000000000)
			bigIntAdd(fractional, &pow);
		fract <<= 1;
	}
	bigIntAdd(fractional, &pow1);
}


void unpackBelowOne(BigInt* integer, BigInt* fractional, uint32_t e, uint64_t m)
{
	uint64_t fract;

	e = 1023 - e;
	fract = m << 12;
	CalcFractional(fractional, fract, e, 0);
	bigIntFromInt(integer, 0);
}

void unpackAverage(BigInt* integer, BigInt* fractional, uint32_t e, uint64_t m)
{
	uint64_t fract;

	m |= 0x0010000000000000;
	bigIntFromInt(integer, m >> (52 - e));
	fract = m << (12 + e);
	CalcFractional(fractional, fract, 0, 0);
}

void unpackHuge(BigInt* integer, BigInt* fractional, uint32_t e, uint64_t m)
{
	BigInt mantiss;
	BigInt pow;

	m |= 0x0010000000000000;
	bigIntFromInt(&mantiss, m);
	nPowY(&pow, 2, e - 52);
	bigIntMultiplyBig(integer, &pow, &mantiss);
}
