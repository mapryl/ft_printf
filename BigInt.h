#ifndef __BIGINT_H__
#define __BIGINT_H__

#include <stdint.h>

# define BIGINT_SIZE 200
# define BIGINT_BASE (1000*1000*1000)

typedef struct
{
	int digits[BIGINT_SIZE];
	size_t size;
} BigInt;

void bigIntCreate(BigInt* bigInt);
void bigIntCreateSize(BigInt* bigInt, int size);
int  bigIntBack(const BigInt* bigInt);
void bigIntPushBack(BigInt* bigInt, int val);
void bigIntPopBack(BigInt* bigInt);
void bigIntAdd(BigInt* a, const BigInt* b);
void bigIntMultiply(BigInt* a, int b);
void bigIntMultiplyBig(BigInt* c, const BigInt* a, const BigInt* b);
void nPowY(BigInt* bigIntResult, int n, int y);
void bigIntDivide(BigInt* a, int b);
int printToStringImp(char* buff, int size, int i, int* curPos);
int printToString(char* buff, int buffSize, int width, int i);
void bigIntPrint(char* buff, int buffSize, const BigInt* bigInt, int isFrac);
int bigIntDecimalSize(const BigInt* bi);
void bigIntFromInt(BigInt* a, int64_t b);

#endif