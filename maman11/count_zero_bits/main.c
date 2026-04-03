#include <stdio.h>
#include "count_zero_bits.h"

int main()
{
	unsigned int num;
	int count;
	printf("Enter one number: \n");
	scanf("%u", &num);
	count = count_zero_bits(num);
	printf("The number is: \"%d\"\n", num);
	printf("The count of zero bits is: \"%d\"\n", count);
return 0;
}
