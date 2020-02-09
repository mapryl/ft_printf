#include <stdlib.h>
#include "BigInt.h"
#include "Utils.h"
#include "DoubleUnpacked.h"
#include "str.h"

int printToStringImp(char* buff, int size, int i, int* curPos)
{
	int cur;

	cur = i % 10;

	if (i >= 10)
		printToStringImp(buff, size, i / 10, curPos);

	if (*curPos < size)
		buff[(*curPos)++] = '0' + cur;

	return *curPos;
}

int printToString(char* buff, int buffSize, int width, int i)
{
	int curPos;
	int iwidth;
	char* tmp;
	int size;

	curPos = 0;
	iwidth = intDecimalSize(i);
	while (width >= 0 && (curPos + iwidth) < width && curPos < buffSize)
		buff[curPos++] = '0';

	if (width >= 0)
		return printToStringImp(buff, buffSize, i, &curPos);
	else
	{
		tmp = strCreate(buffSize + 1);
		strFill(tmp, buffSize + 1, '0');
		size = printToStringImp(tmp, buffSize + 1, i, &curPos);
		strCopy(buff, tmp + 1);
		free(tmp);
		return size - 1;
	}
}

void bigIntPrint(char* buff, int buffSize, const BigInt* bigInt, int isFrac)
{
	int numPrinted;
	int i;

	numPrinted = (bigInt->size == 0)
		? printToString(buff, buffSize, 0, 0)
		: printToString(buff, buffSize, -isFrac, bigIntBack(bigInt));

	i = bigInt->size - 2;
	while (i >= 0 && numPrinted < buffSize)
	{
		numPrinted += printToString(buff + numPrinted, buffSize - numPrinted, 9, bigInt->digits[i]);
		--i;
	}
}