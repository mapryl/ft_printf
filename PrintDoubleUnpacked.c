#include <stdlib.h>
#include "DoubleUnpacked.h"
#include "str.h"
#include "Utils.h"

char* printFractional(const DoubleUnpacked* du, int width)
{
	int fracSize;
	char* str;

	fracSize = width + 1; // last digit will be rounded

	str = strCreate(fracSize);
	strFill(str, fracSize, '0');
	bigIntPrint(str, fracSize, &du->fractional, 1);
	return str;
}

char* printInteger(const DoubleUnpacked* du)
{
	int decSize;
	char* str;

	decSize = bigIntDecimalSize(&du->integer);
	str = strCreate(decSize + du->sign);
	str[0] = '-';
	bigIntPrint(str + du->sign, decSize, &du->integer, 0);

	return str;
}

char* roundHelper(char* dbl, int end)
{
	int carry;

	carry = 1;
	while (--end >= 0)
	{
		if (dbl[end] == '.')
		{
			dbl[end + 1] = '.';
			continue;
		}
		int cur = toInt(dbl[end]) + carry;
		carry = cur / 10;
		dbl[end + 1] = cur % 10 + '0';
	}

	if (carry)
		dbl[0] = '1';
	else
		strShiftLeft(dbl);

	return dbl;
}

char* roundDoubleInplace(char* dbl)
{
	int end;
	int last;

	end = strLen(dbl) - 1;
	last = toInt(dbl[end]);

	dbl[end] = '\0';
	if (last < 5)
		return dbl;

	return roundHelper(dbl, end);
}

char* printDoubleUnpackedNormal(const DoubleUnpacked* du, int fracWidth)
{
	char* integ;
	char* frac;
	char* dblHalf;
	char* dbl;

	integ = printInteger(du);
	frac = printFractional(du, fracWidth);
	dblHalf = strCat(integ, ".");
	dbl = strCat(dblHalf, frac);

	free(integ);
	free(frac);
	free(dblHalf);

	dbl = roundDoubleInplace(dbl);

	return dbl;
}