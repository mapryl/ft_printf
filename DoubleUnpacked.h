#ifndef __DOUBLEUNPACKED_H__
#define __DOUBLEUNPACKED_H__

#include "BigInt.h"

# define DOUBLE_INF 1
# define DOUBLE_NAN 2

typedef struct
{
	int sign;
	BigInt integer;
	BigInt fractional;
	int special; // 0 - nothing special, 1 for , 2 for NaN, 
} DoubleUnpacked;

char* printDoubleUnpackedNormal(const DoubleUnpacked* du, int fracWidth);

#endif
