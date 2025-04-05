#include <string.h>
#include "palindrome.h"

int palindrome(char s[])
{
int start=0, end=strlen(s)-1;
        while (start<end)
        {
		while (!((s[start]>='a' && s[start]<='z') || (s[start]>='A' && s[start]<='Z')))
		{
			start=start+1;
		}
		while (!((s[end]>='a' && s[end]<='z') || (s[end]>='A' && s[end]<='Z')))
		{
			end=end-1;
		}
		if(s[start]!=s[end])
		{
		return 0;
		}
	start=start+1;
	end=end-1;
        }
        return 1;
}
