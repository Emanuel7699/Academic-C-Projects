#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"
#include "firstPass.h"
#include "secondPass.h"
#include "assembler.h"


/**This program processes each file provided as a command-line argumentת
 *check errors and returns the according output.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings (file names).
 * @return file macro expansion, ob file, ent and ext file.
 */
int main (int argc, char *argv[]) {
    int i=0;
    assembler_context *content;

    while (--argc > 0) {
        i++;
        content = malloc(sizeof(assembler_context));
        content->filename = file_extension(argv[i], ".as");
        printf("\nOpen the %s file\n", content->filename);
        if (!read_file(content)) {
            printf("Macro expansion in file \"%s\" completed successfully\n", content->filename);
            printf("Start the first pass\n");
             if (!first_pass(content)){
                printf("First pass completed successfully\n");
                printf("Start the second pass\n");
                if (!second_pass(content)) {
                    printf("Second pass completed successfully\n\n");
                }
                else {
                    printf("The file %s was closed due to errors in the second pass.\n\n", content->filename);
                }
            }
            else {
                printf("The file %s was closed due to errors in the first pass.\n\n", content->filename);
            }
        }
        else {
            printf("The file %s was closed due to errors in the macro expansion.\n\n", content->filename);
        }
        free(content);
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