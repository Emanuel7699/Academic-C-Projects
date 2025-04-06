#include <stdio.h>
#include <string.h>
#include "palindrome.h"
int main()
{
	char s[80];
	int i = 0;
	int c;
	printf("Enter a string: \n");
	while ((c = getchar()) && c != '\n' && i < sizeof(s) - 1 && c != EOF)
	{
        	s[i++] = c;
    	}
    	s[i] = '\0';
	if (s[strlen(s)-1] == '\n'){s[strlen(s)-1] = '\0';}
	printf("The string is: \"%s\"\n", s);

    	if (palindrome(s))
	{
        	printf("The string is a palindrome.\n");
    	} 
	else
	{
        	printf("The string is not a palindrome.\n");
    	}
return 0;
}
