#include <stdio.h>
#include "magic.h"

void sum(int A[][N])
{
int i,j;
int sumRow=0, sumCol=0;

for (i=0; i<N; i++)
{
	for (j=0; j<N; j++)
	{
		sumRow += A[i][j];
		sumCol += A[j][i];
	}
if (sumRow != sumCol){printf("\nThe sum of lines is not equal\n"); return;}
sumRow = 0;
sumCol = 0;
}


for (i=0; i<N; i++)
{
	sumRow += A[i][i];
	sumCol += A[N-1-i][N-1-i];
}
if (sumRow != sumCol){printf("\nThe sum of lines is not equal\n"); return;}


printf("\nThe sum of lines is equal\n");
printf("Matrix:\n");
for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
        printf("%d ", A[i][j]);
    }
    printf("\n");
}
return;
}



