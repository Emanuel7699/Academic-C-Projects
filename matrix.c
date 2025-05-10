#include <stdio.h>
#include "magic.h"

void matrix(int s[])
{
int i,j;
int t=0;
int A[N][N];


for (i=0; i<N; i++)
{
	for (j=0; j<N; j++,t++)
	{
		A[i][j] = (s[t]);
	}
}
sum(A);
return;
}

