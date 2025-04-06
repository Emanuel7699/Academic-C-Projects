#include "count_zero_bits.h"
#define BYTE 8

unsigned int count_zero_bits (unsigned int num)
{

int count = 0;
int i = 0;
int num_bits = sizeof(num) * BYTE;

while (i < num_bits)
{
	if ((num & 1) == 0)
	{
		count++;
	}
	num >>= 1;
	i++;
}

return count;
}
