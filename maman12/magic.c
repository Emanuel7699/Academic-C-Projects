#include <stdio.h>
#include "magic.h"

int main()
{
	int s[N*N];
	int i=0;
	int extra, result=0;

	printf("Enter %d numbers:\n", N*N);
	
	while (i < N*N) {
		result = scanf("%d", &s[i]);
		if (result == 1){
			i++;
		}
		
		else if (result == 0){
			printf("\nThe number that you set is not an integer number\n");
			return 0;
		}
		else if (result == EOF){
			printf("\nYou entered less than %d integers.\n", N*N);
		return 0;
		}
	}

	if (scanf("%d", &extra) == 1) {
		printf("\nYou entered more than %d integers.\n", N*N);
		return 0;
	}

	matrix(s);
return 0;
}

