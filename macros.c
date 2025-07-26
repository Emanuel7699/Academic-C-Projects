#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assembler.h"

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
            close_files(in, out, temp_macro, newfile);
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
                close_files(in, out, temp_macro, newfile);
                return 1;
            }
            if (check_duplicate_macro(line, temp_macro)) {
                close_files(in,out, temp_macro, newfile);
                return 1;
            }
            fputs(line, temp_macro);
            if (write_macro_to_file(in, temp_macro)) {
                close_files(in, out, temp_macro, newfile);
                return 1;
            }
        }
    }
    close_files(in, out, temp_macro, newfile);
    return 0;
}

/*Write the macro to the file temp*/
int write_macro_to_file(FILE *in, FILE *temp_macro) {
    char line[82];
    while (fgets(line, sizeof(line), in) != NULL) {
        fputs(line, temp_macro);
        if (strstr(line, "mcroend") != NULL) {
            if (check_macro(line, 0)) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

/*Write the macro to the file out*/
int check_macro_in_file(char *line, FILE *temp_macro, FILE *out) {
    char macro[10],line_temp[82];
    char macro_def_name[82], macro_name[82];

    rewind(temp_macro);
    sscanf(line, "%s", macro_name);

    while (fgets(line_temp, sizeof(line_temp), temp_macro) != NULL) {
        if (sscanf(line_temp, "%s %s", macro, macro_def_name) == 2) {
            if (strcmp(macro, "mcro") == 0 && strcmp(macro_def_name, macro_name) == 0) {
                while (fgets(line_temp, sizeof(line_temp), temp_macro) != NULL) {
                    if (strstr(line_temp, "mcroend") != NULL) {
                        break;
                    }
                    fputs(line_temp, out);
                }
                return 0;
            }
        }
    }
    return 1;
}

/*check if the macro line is correct*/
int check_macro(char *line, int i) {
    char macro[10], macro_name[82];
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

/*duplicate macro*/
int check_duplicate_macro(char *line, FILE *temp_macro) {
    char line_temp[82];
    rewind(temp_macro);
    while (fgets(line_temp, sizeof(line_temp), temp_macro) != NULL) {
        if (strcmp(line_temp, line) == 0) {
            fprintf(stderr, "Error: Duplicate macros\n");
            return 1;
        }
    }
    return 0;
}

/*close files*/
void close_files(FILE *in, FILE *out, FILE *temp_macro, char *newfile) {
    if (temp_macro) {fclose(temp_macro); remove("temp_macro.am");}
    if (in) fclose(in);
    if (out) fclose(out);
    if (newfile) free(newfile);
}