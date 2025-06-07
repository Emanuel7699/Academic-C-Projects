#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "numbers.h"

int main (int argc, char *file[])
{
	int num=0,i=0;
	FILE *in1 = NULL;
	FILE *in2 = NULL;
	char *text;

	if (argc > 3) {
        fprintf(stderr,"Error: you insert more than 2 files.\n");
        return 1;
    }
	in1 = fopen(file[1], "r");
	in2 = fopen(file[2], "r");
	
	if (argc == 1){/*If there is no file.*/
		printf("enter some numbers\n");

		while (scanf("%d", &num) != EOF){
			if (i==0){
				printf("the numbers is:\n");
				i++;
			}
			text = malloc(50);
			text[0] = '\0';
			printf("%s\n",translate(&num,text));
			free(text);
		}
	}
	else if(argc == 2){/*If there is no input file.*/
		if (in1 == NULL){/*If the file is incorrect.*/
			fprintf(stderr,"ERROR: the file incorrrect or can't be opened\n");
			return 0;
		}
		printf("the numbers is:\n");
		while (fscanf(in1,"%d", &num) != EOF){
			text = malloc(50);
			text[0] = '\0';
			printf("%s\n",translate(&num,text));
			free(text);
		}
		fclose(in1);
	}
	else{/* If there is no output file.*/
		if (in1 == NULL){/*If the file is incorrect.*/
			fprintf(stderr,"ERROR: the file incorrrect or can't be opened\n");
			return 0;
		}
		if (in2 == NULL){/*If the file is incorrect.*/
			fprintf(stderr,"ERROR: the file incorrrect or can't be opened\n");
			return 0;
		}
		fclose(in2);
		in2 = fopen(file[2], "w");
		fprintf(in2,"the numbers is:\n");
		while (fscanf(in1,"%d", &num) != EOF){
			text = malloc(50);
			text[0] = '\0';
			fprintf(in2,"%s\n",translate(&num,text));
			free(text);
		}
		fclose(in1);
		fclose(in2);
	}
	return 0;
}

char *translate(int *num,char *text){
	char *numbers1[20]={"zero","one","two","three","four","five","six","seven","eight","nine","ten",
						"eleven","twelve","thirteen","fourteen","fifteen",
						"sixteen","seventeen","eighteen","nineteen"};
	char *numbers2[8]={"twenty","thirty","fourty","fifty","sixty","seventy","eighty","ninety"};
	
	if(*num<20){
		return numbers1[*num];
	}
	else if((*num%10)==0){
		return numbers2[*num/10-2];
	}
	else{
		strcat(text,numbers2[*num/10-2]);
		strcat(text, " ");
		strcat(text,numbers1[*num%10]);
		return text;
	}
	return "";
}