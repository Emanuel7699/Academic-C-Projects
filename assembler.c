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
        free(file);
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
    int flag = 1;
    FILE *in = NULL, *out = NULL, *temp_macro = NULL;
    char line[80],line_temp[80],macro_line[80];
    char *newfile = file_extension(filename,".am");

    in = fopen(filename, "r");
    out = fopen(newfile, "w");
    temp_macro = fopen("temp_macro.am", "w+");


    while (fgets(line, sizeof(line), in) != NULL) {
        if (strstr(line, "mcro") == NULL) {/*Not macro*/
            rewind(temp_macro);
            sprintf(macro_line, "mcro %s", line);

            while (fgets(line_temp, sizeof(line_temp), temp_macro) != NULL) {
                if (strstr(line_temp, macro_line) != NULL){/*if the line appear in "mcro line"*/
                    flag = 0;
                    while (fgets(line_temp, sizeof(line_temp), temp_macro) != NULL) {
                        if (strstr(line_temp, "mcroend") != NULL) {
                            break;
                        }
                        fputs(line_temp, out);
                    }
                    break;
                }
            }
            if (flag){
                fputs(line, out);
            }
            flag = 1;
        }
        else {/*Macro*/
            fputs(line, temp_macro);
            while (fgets(line, sizeof(line), in) != NULL) {
                fputs(line, temp_macro);
                if (strstr(line, "mcroend") != NULL) {
                    break;
                }
            }
        }
    }
    fclose(temp_macro);
    fclose(in);
    fclose(out);
    free(newfile);
    remove("temp_macro.am");
    return 0;
}