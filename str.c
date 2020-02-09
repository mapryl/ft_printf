#include <stdlib.h>
#include "str.h"

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