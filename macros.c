#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assembler.h"

/*Expand the macros in the file*/
int read_file(char *filename) {
    FILE *in = NULL, *out = NULL, *temp_macro = NULL;
	char macro[10], macro_name[82];
    char line[82];
    int len, error = 0, lineNumber = 1;
    char *newfile = file_extension(filename,".am");

    in = fopen(filename, "r");
    if (!in) {/*if the input file can't be open*/
        printf("Error: Cannot open input file\n");
        return 1;
    }
    out = fopen(newfile, "w");
    temp_macro = fopen("temp_macro.am", "w+");

    while (fgets(line, sizeof(line), in) != NULL) {
        len = strlen(line);
        if ((len > 80) || (len == 80 && line[len-1] != '\n')) {
            printf("Error: in line %d- Line too long (more than 80 characters)\n", lineNumber);
            error = 1;
        }

        if (strncmp(line, ";", 1) == 0) {/*if there is a comment line*/
            continue;
        }

        if (strstr(line, "mcro") == NULL) {/*If the line is not a macro*/
            if (check_macro_in_file(line, temp_macro, out) == 0) {/*check if the line is a macro call*/
                fputs(line, out);
            }
        }
        else {/*If the line is a macro*/
			sscanf(line, "%s %s", macro, macro_name);
            if (check_macro(line, 1, &lineNumber) == 1) {
                error = 1;
            }
            if (check_duplicate_macro(macro, macro_name, temp_macro) == 1) {
				printf("Error: in line %d- Duplicate macros\n", lineNumber);
                error = 1;
            }
            if (check_instruction(line) == 1) {
				printf( "Error: in line %d- The command is instruction\n", lineNumber);
                error = 1;
            }

            fputs(line, temp_macro);
            if (write_macro_to_file(in, temp_macro, &lineNumber)) {
                error = 1;
            }
        }
        lineNumber++;
    }
    close_files(in, out, temp_macro);
    if (error > 0) {
        remove(newfile);
        free(newfile);
        return 1;
    }
    free(newfile);
    return 0;
}

/*Write the macro to the file temp*/
int write_macro_to_file(FILE *in, FILE *temp_macro, int *lineNumber) {
    char line[82];
    while (fgets(line, sizeof(line), in) != NULL) {
        fputs(line, temp_macro);
        if (strstr(line, "mcroend") != NULL) {
            if (check_macro(line, 0, lineNumber) == 1) {
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
int check_macro(char *line, int i, int *lineNumber) {
    char macro[10], macro_name[82];
    char *extra;

    if (i == 1) {
        sscanf(line, "%s %s", macro, macro_name);
        extra = (strstr(line, macro_name) + strlen(macro_name));
        if (strcmp(macro, "mcro") != 0 || (*extra != '\r' && *extra != '\n' && *extra != '\0')){
            printf("Error: in line %d- Invalid macro definition line\n", *lineNumber);
            return 1;
        }
    }
    if (i == 0) {
        sscanf(line, "%s", macro);
        extra = (strstr(line, macro) + strlen(macro));
        if (strcmp(macro, "mcroend") != 0 || (*extra != '\r' && *extra != '\n' && *extra != '\0')) {
            printf("Error: in line %d- Invalid endmcro line\n", *lineNumber);
            return 1;
        }
    }
    return 0;
}

/*duplicate macro*/
int check_duplicate_macro(char *macro, char *macro_name, FILE *temp_macro) {
    char line_temp[82], macro_temp[82], macro_name_temp[82];
    rewind(temp_macro);
    while (fgets(line_temp, sizeof(line_temp), temp_macro) != NULL) {
		sscanf(line_temp, "%s %s", macro_temp, macro_name_temp);
		if (strcmp(macro, macro_temp) == 0 && strcmp(macro_name, macro_name_temp) == 0){
            return 1;
        }
    }
    return 0;
}

int check_instruction(char *macro) {
    if (strstr(macro, "mov") != NULL ||
        strstr(macro, "cmp") != NULL ||
        strstr(macro, "add") != NULL ||
        strstr(macro, "sub") != NULL ||
        strstr(macro, "lea") != NULL ||
        strstr(macro, "clr") != NULL ||
        strstr(macro, "not") != NULL ||
        strstr(macro, "inc") != NULL ||
        strstr(macro, "dec") != NULL ||
        strstr(macro, "jmp") != NULL ||
        strstr(macro, "bne") != NULL ||
        strstr(macro, "jsr") != NULL ||
        strstr(macro, "red") != NULL ||
        strstr(macro, "prn") != NULL ||
        strstr(macro, "rts") != NULL ||
        strstr(macro, "stop") != NULL ||
        strstr(macro, "data") != NULL ||
        strstr(macro, "string") != NULL ||
        strstr(macro, "mat") != NULL ||
        strstr(macro, "extern") != NULL ||
        strstr(macro, "entry") != NULL) {
        return 1;
        }
    return  0;
}

/*close files*/
void close_files(FILE *in, FILE *out, FILE *temp_macro) {
    fclose(in);
    fclose(out);
    fclose(temp_macro);
}