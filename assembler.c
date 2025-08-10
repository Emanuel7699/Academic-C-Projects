#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assembler.h"
#include "firstPass.h"

/*The main function that receive files and return the file with macros*/
int main (int argc, char *argv[]) {
    int i=0;
    char *file;

    while (--argc > 0) {
        i++;
        file = file_extension(argv[i], ".as");
		printf("Open the %s file\n", file);
        if (!read_file(file)) {
            if (first_pass(file)){
				/*second_pass(file);*/
			}
        }
        free(file);
    }
    return 0;
}

/*Add the correct extension to the file*/
char *file_extension(char *filename, char *ending) {
    char *dot, *newname;
    newname =  (char *)malloc(strlen(filename) + 4);
    strcpy(newname, filename);

    if ((dot = strchr(newname, '.')) != NULL) {
        *dot = '\0';
    }

    strcat(newname, ending);
    return newname;
}