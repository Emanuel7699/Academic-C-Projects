#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assembler.h"

/*The main function that receive files and return the file with macros*/
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

/*Expand the macros in the file*/
int read_file(char *filename) {
    FILE *in = NULL, *out = NULL, *temp_macro = NULL;
    char line[82];
    int len;
    char *newfile = file_extension(filename,".am");

    in = fopen(filename, "r");
    out = fopen(newfile, "w");
    temp_macro = fopen("temp_macro.am", "w+");
    if (!in) {/*if the input file can't be open*/
        fprintf(stderr, "Error: Cannot open input file\n");
        return 1;
    }

    while (fgets(line, sizeof(line), in) != NULL) {
        len = strlen(line);
        if ((len > 80) || (len == 80 && line[len-1] != '\n')) {
            fprintf(stderr, "Error: Line too long (more than 80 characters)\n");
            return 1;
        }

        if (strncmp(line, ";", 1) == 0) {/*if there is a comment line*/
            continue;
        }

        if (strstr(line, "mcro") == NULL) {/*If the line is not a macro*/
            if (check_macro_in_file(line, temp_macro, out)) {/*check if the line is a macro call*/
                fputs(line, out);
            }
        }
        else {/*If the line is a macro*/
            if (check_macro(line, 1)) {
                return 1;
            }
            if (check_duplicate_macro(line, temp_macro)) {
                return 1;
            };
            fputs(line, temp_macro);
            if (write_macro_to_file(in, temp_macro)) {
                return 1;
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

/*Write the macro to the file temp*/
int write_macro_to_file(FILE *in, FILE *temp_macro) {
    char line[80];
    while (fgets(line, sizeof(line), in) != NULL) {
        fputs(line, temp_macro);
        if (strstr(line, "mcroend") != NULL) {
            if (!check_macro(line, 0)) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

/*Write the macro to the file out*/
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
    return 1; /* Macro not found */
}


int check_macro(char *line, int i) {
    char macro[10], macro_name[80];
    char *extra;

    if (i == 1) {
        sscanf(line, "%s %s", macro, macro_name);
        extra = (strstr(line, macro_name) + strlen(macro_name));
        if (strcmp(macro, "mcro") != 0 || (*extra != '\r' && *extra != '\n' && *extra != '\0')){
            fprintf(stderr, "Error: Invalid macro definition line\n");
            return 1;
        }
    }
    if (i == 0) {
        sscanf(line, "%s", macro);
        extra = (strstr(line, macro) + strlen(macro));
        if (*extra != '\r' && *extra != '\n' && *extra != '\0') {
            fprintf(stderr, "Error: Invalid endmcro line\n");
            return 1;
        }
    }

    return 0;
}

int check_duplicate_macro(char *line, FILE *temp_macro) {
    char line_temp[80];
    rewind(temp_macro);
    while (fgets(line_temp, sizeof(line_temp), temp_macro) != NULL) {
        if (strcmp(line_temp, line) == 0) {
            fprintf(stderr, "Error: Duplicate macros\n");
            return 1;
        }
    }
    return 0;
}