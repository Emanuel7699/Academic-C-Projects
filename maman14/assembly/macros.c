#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"
#include "assembler.h"


/** This program expand the macro in the file.
 *
 * @param content a struct which centralizes all the variables that pass between functions.
 * @return The file macro expansion.
 */
int read_file(assembler_context *content) {
    FILE *temp_macro = NULL;
    int len;
    char *newfile = file_extension(content->filename,".am");
    content->out = NULL;
    content->error = 0;
    content->lineNumber = 1;
    content->in = fopen(content->filename, "r");
    if (!content->in) {/*if the input file can't be open*/
        printf("Error: Cannot open input file\n");
        return 1;
    }
    content->out = fopen(newfile, "w");
    temp_macro = fopen("temp_macro.am", "w+");

    while (fgets(content->line, sizeof(content->line), content->in) != NULL) {
        len = strlen(content->line);
        if ((len > 80) || (len == 80 && content->line[len-1] != '\n')) {
            printf("Error: in line %d- Line too long (more than 80 characters)\n", content->lineNumber);
            content->lineNumber--;
            content->error = 1;
        }

        if (strncmp(content->line, ";", 1) == 0) {/*if there is a comment line*/
            content->lineNumber++;
            continue;
        }

        if (strstr(content->line, "mcro") == NULL) {/*If the line is not a macro*/
            if (check_macro_in_file(content, temp_macro) == 1) {/*check if the line is a macro call*/
                fputs(content->line, content->out);
            }
        }
        else {/*If the line is a macro*/
			sscanf(content->line, "%s %s", content->macro, content->macroName);
            if (check_macro(1, content) == 1) {
                content->error = 1;
            }
            if (check_duplicate_macro(content, temp_macro) == 1) {
				printf("Error: in line %d- Duplicate macros\n",content->lineNumber);
                content->error = 1;
            }
            if (check_instruction(content->macroName) == 1) {
				printf( "Error: in line %d- The command is instruction\n", content->lineNumber);
                content->error = 1;
            }

            fputs(content->line, temp_macro);
            if (write_macro_to_file(content, temp_macro) == 1) {
                content->error = 1;
            }
        }
        content->lineNumber++;
    }
    close_files(content->in, content->out, temp_macro);
    if (content->error > 0) {
        remove(newfile);
        return 1;
    }
    return 0;
}

/*Write the macro to the file temp*/
int write_macro_to_file(assembler_context *content, FILE *temp_macro) {
    while (fgets(content->line, sizeof(content->line), content->in) != NULL) {
        content->lineNumber++;
        fputs(content->line, temp_macro);
        if (strstr(content->line, "mcroend") != NULL) {
            if (check_macro(0, content) == 1) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

/*Write the macro to the file out*/
int check_macro_in_file(assembler_context *content, FILE *temp_macro) {
    char macro[10], line_temp[82];
    char macro_def_name[82], macro_name[82];

    rewind(temp_macro);
    sscanf(content->line, "%s", macro_name);

    while (fgets(line_temp, sizeof(line_temp), temp_macro) != NULL) {
        if (sscanf(line_temp, "%s %s", macro, macro_def_name) == 2) {
            if (strcmp(macro, "mcro") == 0 && strcmp(macro_def_name, macro_name) == 0) {
                while (fgets(line_temp, sizeof(line_temp), temp_macro) != NULL) {
                    if (strstr(line_temp, "mcroend") != NULL) {
                        break;
                    }
                    fputs(line_temp, content->out);
                }
                return 0;
            }
        }
    }
    return 1;
}

/*Check if the macro line is correct*/
int check_macro(int i, assembler_context *content) {
    char *extra;

    if (i == 1) {
        sscanf(content->line, "%s %s", content->macro, content->macroName);
        extra = (strstr(content->line, content->macroName) + strlen(content->macroName));
        if (strcmp(content->macro, "mcro") != 0 || (*extra != '\r' && *extra != '\n' && *extra != '\0')){
            printf("Error: in line %d- Invalid macro definition line\n", content->lineNumber);
            return 1;
        }
    }
    else if (i == 0) {
        sscanf(content->line, "%s", content->macro);
        extra = (strstr(content->line, content->macro) + strlen(content->macro));
        if (strcmp(content->macro, "mcroend") != 0 || (*extra != '\r' && *extra != '\n' && *extra != '\0')) {
            printf("Error: in line %d- Invalid endmcro definition line\n", content->lineNumber);
            return 1;
        }
    }
    return 0;
}

/*Duplicate macro*/
int check_duplicate_macro(assembler_context *content, FILE *temp_macro) {
    char line_temp[82], macro_temp[82], macro_name_temp[82];
    rewind(temp_macro);
    while (fgets(line_temp, sizeof(line_temp), temp_macro) != NULL) {
		sscanf(line_temp, "%s %s", macro_temp, macro_name_temp);
		if (strcmp(content->macroName, macro_temp) == 0 && strcmp(content->labelName, macro_name_temp) == 0){
            return 1;
        }
    }
    return 0;
}

/*Checks if the parent exists in the instruction table*/
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

/*Close files*/
void close_files(FILE *in, FILE *out, FILE *temp_macro) {
    fclose(in);
    fclose(out);
    fclose(temp_macro);
}