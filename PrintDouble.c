#include <stdlib.h>
#include "str.h"
#include "DoubleUnpacked.h"
#include "UnpackDouble.h"

char* correctTotalWidth(char* str, int totalWidth)
{
	int size;
	char* newStr;

	size = strLen(str);
	if (totalWidth <= size)
		return str;

	newStr = strCreate(totalWidth);
	strFill(newStr, totalWidth, ' ');
	strCopy(newStr + totalWidth - size, str);
	free(str);

	return newStr;
}

char* printDoubleUnpacked(const DoubleUnpacked* du, int totalWidth, int fracWidth)
{
	char* str;

	if (du->special == DOUBLE_NAN || du->special == DOUBLE_INF)
	{
		str = strCreate(3 + du->sign);
		str[0] = '-';
		strCopy(str + du->sign, (du->special == DOUBLE_NAN) ? "nan" : "inf");
	}
	else
		str = printDoubleUnpackedNormal(du, fracWidth);

	str = correctTotalWidth(str, totalWidth);
	return str;
}

char* printDouble(double d, int totalWidth, int fractionalWidth)
{
	DoubleUnpacked du;

	du = unpackDouble(d);
	return printDoubleUnpacked(&du, totalWidth, fractionalWidth);
}