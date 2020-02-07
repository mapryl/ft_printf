#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void strCopy(char* dst, const char* src);

# define BIGINT_SIZE 200
# define BIGINT_BASE (1000*1000*1000)

typedef struct
{
	int digits[BIGINT_SIZE];
	int size;
} BigInt;

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

int intDecimalSize(int i)
{
	int size;

	size = 1;
	while (i /= 10)
		++size;

	return size;
}

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

char* strCreate(int size)
{
	char* str;

	str = (char*)malloc((size + 1) * sizeof(char));
	str[size] = '\0';

	return str;
}

void strFill(char* str, int size, char ch)
{
	int i;

	i = 0;
	while (i < size)
		str[i++] = ch;
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

int myMax(int a, int b)
{
	return (a > b) ? a : b;
}

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
	int j;
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

#define DOUBLE_INF 1
#define DOUBLE_NAN 2

typedef struct
{
	int sign;
	BigInt integer;
	BigInt fractional;
	int special; // 0 - nothing special, 1 for , 2 for NaN, 
} DoubleUnpacked;


DoubleUnpacked assignDoubleUnpacked(int sign, BigInt* integer, BigInt* fractional, int special)
{
	DoubleUnpacked result;
	
	result.sign = sign;
	result.integer = *integer;
	result.fractional = *fractional;
	result.special = special;

	return result;
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
		if(!denormal)
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

void unpackDenormal(BigInt* integer, BigInt* fractional, uint64_t mantiss)
{
	bigIntFromInt(integer, 0);
	uint64_t fract = mantiss << 12;
	CalcFractional(fractional, fract, 1022, 1);
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
	DoubleUnpacked result;

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

int bigIntDecimalSize(const BigInt* bi)
{
	return intDecimalSize(bi->digits[bi->size - 1]) + 9 * (bi->size - 1);
}

char* printInteger(const DoubleUnpacked* du)
{
	int decSize;
	char* str;

	decSize = bigIntDecimalSize(&du->integer);
	str = strCreate(decSize);
	bigIntPrint(str, decSize, &du->integer, 0);

	return str;
}

int strLen(const char* str)
{
	int size;

	size = 0;
	while (*(str + size))
		++size;

	return size;
}

void strCopy(char* dst, const char* src)
{
	while (*(dst++) = *(src++));
}

char* strCat(const char* str1, const char* str2)
{
	int size1;
	int size2;
	char* str;

	size1 = strLen(str1);
	size2 = strLen(str2);

	str = strCreate(size1 + size2);
	strCopy(str, str1);
	strCopy(str + size1, str2);

	return str;
}

int toInt(char ch)
{
	return ch - '0';
}

void strShiftLeft(char* str)
{
	while (*str = *(str + 1))
		++str;
}

char* roundHelperSuka(char* dbl, int end)
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

	return roundHelperSuka(dbl, end);
}

char* correctTotalWidth(char* str, int totalWidth)
{
	int size;
	char* newStr;

	size = strLen(str);
	if (size <= totalWidth)
		return str;

	newStr = strCreate(size);
	strFill(newStr, size, ' ');
	strCopy(newStr + size - totalWidth, str);
	free(str);

	return newStr;
}

void printDoubleUnpackedNormal(const DoubleUnpacked* du, int fracWidth)
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
	dbl = correctTotalWidth(dbl);

	printf("%s", dbl);

	free(dbl);
}

void printDoubleUnpacked(const DoubleUnpacked* du, int fracWidth)
{
	if(du->sign)
		printf("%s", "-");

	if (du->special == DOUBLE_NAN)
		printf("%s", "nan");
	else if (du->special == DOUBLE_INF)
		printf("%s", "inf");
	else
		printDoubleUnpackedNormal(du, fracWidth);
}

void printDouble(double d, int totalWidth, int fractionalWidth)
{
	DoubleUnpacked du = unpackDouble(d);
	printDoubleUnpacked(&du, fractionalWidth);
}

int main()
{
	double d = 0.1213;

	printf("system print: %1.9f\n", d);

	printf("my printf:    ");
	printDouble(d, 0, 9);
}
