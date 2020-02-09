#ifndef __UNPACKDOUBLE_H__
#define __UNPACKDOUBLE_H__

#include "DoubleUnpacked.h"

DoubleUnpacked unpackDouble(double d);
void CalcFractional(BigInt* fractional, uint64_t fract, int initialPow, int denormal);
void unpackBelowOne(BigInt* integer, BigInt* fractional, uint32_t e, uint64_t m);
void unpackAverage(BigInt* integer, BigInt* fractional, uint32_t e, uint64_t m);
void unpackHuge(BigInt* integer, BigInt* fractional, uint32_t e, uint64_t m);

#endif
