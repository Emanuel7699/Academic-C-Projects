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
    FILE *in = NULL, *out = NULL, *temp_macro = NULL;
    char line[80];
    char *newfile = file_extension(filename,".am");

    in = fopen(filename, "r");
    out = fopen(newfile, "w");
    temp_macro = fopen("temp_macro.am", "w+");

    while (fgets(line, sizeof(line), in) != NULL) {
        if (strstr(line, "mcro") == NULL) {/*Not macro*/
           if (check_macro_in_file(line, temp_macro, out)) {
               fputs(line, out);
           }
        }
        else {/*Macro*/
            fputs(line, temp_macro);
            write_macro_to_file(in, temp_macro);
        }
    }
    fclose(temp_macro);
    fclose(in);
    fclose(out);
    free(newfile);
    remove("temp_macro.am");
    return 0;
}

int write_macro_to_file(FILE *in, FILE *temp_macro) {
    char line[80];

    while (fgets(line, sizeof(line), in) != NULL) {
        fputs(line, temp_macro);
        if (strstr(line, "mcroend") != NULL) {
            break;
        }
    }
    return 0;
}

int check_macro_in_file(char *line, FILE *temp_macro, FILE *out) {
    char line_temp[80];
    char macro_line[80];

    rewind(temp_macro);
    sprintf(macro_line, "mcro %s", line);

    while (fgets(line_temp, sizeof(line_temp), temp_macro) != NULL) {
        if (strstr(line_temp, macro_line) != NULL) {
            while (fgets(line_temp, sizeof(line_temp), temp_macro) != NULL) {
                if (strstr(line_temp, "mcroend") != NULL) {
                    break;
                }
                fputs(line_temp, out);
            }
            return 0; /* Macro found and expanded */
        }
    }
    return -1; /* Macro not found */
}