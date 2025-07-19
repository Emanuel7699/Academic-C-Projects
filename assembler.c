#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assembler.h"


int main (int argc, char *argv[]) {
    int i=0;
    char *file;

    while (--argc > 0) {
        i++;
        file = file_extension(argv[i], ".as");
        read_file(file);
    }
    return 0;
}


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


int read_file(char *filename){
    int line_number=1;
    FILE *in = NULL;
    FILE *out = NULL;
    FILE *temp_macro = NULL;
    char line[256],line_temp[256];
    char *newfile = file_extension(filename,".am");
    in = fopen(filename, "r");
    out = fopen(newfile, "w");
    temp_macro = fopen("temp_macro.am", "w");
    while (fgets(line, sizeof(line), in) != NULL) {
        if (strstr(line, "mcro") == NULL) {
            while (fgets(line_temp, sizeof(line_temp), temp_macro) != NULL) {
                if (strstr(line_temp, line) != NULL) {
                    fputs(line_temp, out);
                }
            }
            fputs(line, out);
        }
        else {
            fprintf(temp_macro, "%d %s", line_number, line);
            while (fgets(line, sizeof(line), in) != NULL)  {
                if (strstr(line, "mcroend") != NULL) {
                    break;
                }
                /*if (strstr(line, "mcroend") != NULL) {
                    fputs(line, temp_macro);
                    break;
                }
                fputs(line, temp_macro);*/
            }
        }
        if (strstr(line, "...") != NULL) {
            while (fgets(line, sizeof(line), in) != NULL)
            fputs(line, out);
        }
        line_number++;
    }
    fclose(in);
    fclose(out);
    return 0;
}