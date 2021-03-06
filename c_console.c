#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "PrintDouble.h"


////////////////////////////////////////////////////////////////
////////////////////////// for testing /////////////////////////////////////
////////////////////////////////////////////////////////////////

#include <math.h>
#include "Utils.h"
#include "str.h"

int strCmp(const char* s1, const char* s2)
{
	while ( (*(s1++) == *(s2++)) && *(s1 - 1));
	return *(s1 - 1) == *(s2 - 1);
}

void test(double d, int totalWidth, int fracWidth)
{
	char buf[1024];
	
	int size = intDecimalSize(totalWidth) + intDecimalSize(fracWidth) + 3;
	char* strFormat = strCreate(size);
	strFill(strFormat, size, '%');

	int pos = 1;
	printToStringImp(strFormat, size, totalWidth, &pos);
	strCopy(strFormat + (pos++), ".");
	printToStringImp(strFormat, size, fracWidth, &pos);
	strCopy(strFormat + pos, "f");

	sprintf(buf, strFormat, d);
	free(strFormat);

	char* test = printDouble(d, totalWidth, fracWidth);
	if ( !strCmp(buf, test) )
	{
		printf("test failed:\n%s\n%s", buf, test);
		free(test);
		exit(1);
	}

	printf("test OK: %s\n", buf);
	free(test);
}

int main()
{
	test(-0.0, 17, 33);
	test(0.0, 1, 1);

	test(-0.1213, 13, 4);
	test(0.55555, 13, 17);

	test(0.00000000005, 1, 77);
	test(-0.00000000005, 1, 77);

	test(7876887.568634534, 66, 33);
	test(-7876887.568634534, 66, 33);

	test(3657677658587587696745.1243, 13, 17);
	test(-3657677658587587696745.1243, 13, 17);

	test(3.3e-317, 13, 570);
	test(-3.3e-317, 13, 570);

	test(1.2e300, 1, 22);
	test(-1.2e300, 1, 22);

	double d1 = 1.0;
	double d2 = 0.0;
	test(d1 / d2, 100, 12);
	test(-d1 / d2, 100, 12);
	
	typedef union
	{
		uint64_t uint;
		double d;
	} IntDou;

	IntDou id;
	id.uint = 0x7FFFFFFFFFFFFFFF;
	test(id.d, 1, 1);

	id.uint = 0xFFFFFFFFFFFFFFFF;
	test(id.d, 1, 1);

	id.uint = 0x7FFF000000000001;
	test(id.d, 1, 1);
}