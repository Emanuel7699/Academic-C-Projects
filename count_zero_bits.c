#include "count_zero_bits.h"

int count_zero_bits (int num)
{

int count = 0;

while (num > 0)
{
	if (num%2 == 0){count++;}
	num = num/2;
}
return count;
}
