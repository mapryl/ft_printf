#include "UnpackDouble.h"
#include "BigInt.h"
#include "Utils.h"
#include "DoubleUnpacked.h"

DoubleUnpacked assignDoubleUnpacked(int sign, BigInt* integer, BigInt* fractional, int special)
{
	DoubleUnpacked result;

	result.sign = sign;
	result.integer = *integer;
	result.fractional = *fractional;
	result.special = special;

	return result;
}

void unpackDenormal(BigInt* integer, BigInt* fractional, uint64_t mantiss)
{
	bigIntFromInt(integer, 0);
	uint64_t fract = mantiss << 12;
	CalcFractional(fractional, fract, 1022, 1);
}

void unpackNormal(BigInt* integer, BigInt* fractional, uint64_t representation, uint64_t m)
{
	uint32_t e;

	e = (int32_t)((representation & 0x7FF0000000000000) >> 52);
	if (e < 1023)
		unpackBelowOne(integer, fractional, e, m);
	else {
		e -= 1023;
		if (e <= 52)
			unpackAverage(integer, fractional, e, m);
		else
			unpackHuge(integer, fractional, e, m);
	}
}

DoubleUnpacked unpackDouble(double d)
{
	uint64_t rep;
	BigInt fractional;
	BigInt integer;
	int special;

	rep = *(uint64_t*)(&d);
	bigIntCreate(&fractional);
	bigIntCreate(&integer);
	special = 0;

	if (rep == 0x0 || rep == 0x8000000000000000)
	{
		bigIntFromInt(&integer, 0);
		bigIntFromInt(&fractional, 0);
	}
	else if (rep == 0x7FF0000000000000 || rep == 0xFFF0000000000000) // inf
		special = DOUBLE_INF;
	else if ((rep & 0xFFF0000000000000) == 0x7FF0000000000000 || (rep & 0xFFF0000000000000) == 0xFFF0000000000000) // NaN
		special = DOUBLE_NAN;
	else if ((rep & 0xFFF0000000000000) == 0x8000000000000000 || (rep & 0xFFF0000000000000) == 0) //denormalized
		unpackDenormal(&integer, &fractional, rep & 0x000FFFFFFFFFFFFF);
	else
		unpackNormal(&integer, &fractional, rep, rep & 0x000FFFFFFFFFFFFF);
	return assignDoubleUnpacked(rep >> 63, &integer, &fractional, special);
}