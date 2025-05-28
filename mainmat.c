#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mymat.h"

mat MAT_A, MAT_B, MAT_C, MAT_D, MAT_E, MAT_F;

mat* name_of_matrix(char* name){
	if (name==NULL){printf("Missing argument\n");return NULL;}
	if (strcmp(name, "MAT_A") == 0) return &MAT_A;
	if (strcmp(name, "MAT_B") == 0) return &MAT_B;
	if (strcmp(name, "MAT_C") == 0) return &MAT_C;
	if (strcmp(name, "MAT_D") == 0) return &MAT_D;
	if (strcmp(name, "MAT_E") == 0) return &MAT_E;
	if (strcmp(name, "MAT_F") == 0) return &MAT_F;
	if (strstr(name, " ") != 0){printf("Missing comma\n"); return 0;}
	printf("Undefined matrix name\n");
	return NULL;
}


int main() {
	char input[300];
	char input_copy[300];
	ParsedCommand *parsed, *error;


	parsed = malloc(sizeof(ParsedCommand));
	error = malloc(sizeof(ParsedCommand));
	parsed->values = malloc(sizeof(float) * 16);
	parsed->matrixs = malloc(sizeof(mat *) * 6);
	error->values = malloc(sizeof(float) * 16);
	error->matrixs = malloc(sizeof(mat *) * 6);

	while(1){
		printf("Please enter a command:\n");
		if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("error: The program ended without stopped.\n");
            break;
        }
		input[strcspn(input, "\n")] = '\0';
		strcpy(input, trim(input));
		strcpy(input_copy, input);
		parsed->command = strtok(input, " ");
		error->command = parsed->command;
		
		if(errors(error, input_copy)){
			int i=0;
			if (strcmp(parsed->command, "read_mat") == 0) {
				*(parsed->matrixs) = *(error->matrixs);
					
				while (i<16) {
					*(parsed->values+i) = *(error->values+i);
					i++;
				}
				read_mat(parsed);
			}
		
			else if (strcmp(parsed->command, "add_mat") == 0){
				while(i<3){
					*(parsed->matrixs+i) = *(error->matrixs+i);
					i++;
				}
				add_mat(parsed);
				}
		
			else if (strcmp(parsed->command, "sub_mat") == 0){
				while(i<3){
					*(parsed->matrixs+i) = *(error->matrixs+i);
					i++;
				}
				sub_mat(parsed);
			}
			
			else if (strcmp(parsed->command, "mul_mat") == 0){
				while(i<3){
					*(parsed->matrixs+i) = *(error->matrixs+i);
					i++;
				}
				mul_mat(parsed);
			}	
		
			else if (strcmp(parsed->command, "mul_scalar") == 0){
				*(parsed->matrixs) = *(error->matrixs);
				*(parsed->values) = *(error->values);
				*(parsed->matrixs+1) = *(error->matrixs+1);
				mul_scalar(parsed);
			}
		
			else if (strcmp(parsed->command, "trans_mat") == 0){
				while(i<2){
					*(parsed->matrixs+i) = *(error->matrixs+i);
					i++;
				}
				trans_mat(parsed);
			}
		
			else if (strcmp(parsed->command, "print_mat") == 0){	
				*(parsed->matrixs) = *(error->matrixs);
				print_mat(*(parsed->matrixs));
			}
		
			else if (strcmp(parsed->command, "stop") ==0){
				printf("stopped\n");
				break;
			}
		}
	}
	free(parsed->values);
	free(parsed->matrixs);
	free(parsed);

	free(error->values);
	free(error->matrixs);
	free(error);
	return 0;
}

char* trim(char *str) {/*Clean spaces and tabs.*/
	char *end;
    if (str == NULL) {
		printf("Missing argument\n");
		return 0;
	}

    while (isspace(*str)) {
        str++;
    }

    if (*str == '\0') {
        return str;
    }
    
	end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) {
        end--;
    }

    *(end + 1) = '\0';
    return str;
}




int errors(ParsedCommand *error,char *input) {/*Check all errors.*/
    char *token;
    int i = 0;
    float temp;
	char extra;
	
	if (error->command == NULL ) {
		printf("Missing argument\n");
		return 0;
	}
	
	if (strstr(input, ",,") != 0) {
		printf("Multiple consecutive commas\n");
		return 0;
	}
	
    if (strcmp(error->command, "read_mat") == 0) {
        *(error->matrixs) = name_of_matrix(trim(strtok(NULL, ",")));
        if (!*(error->matrixs)){
			
			return 0;
		}			

        while (i < 16) {
			if ((token = strtok(NULL, ",")) != NULL){
				if (sscanf(token, "%f %c", &temp, &extra) != 1) {
					printf("Argument is not a real number\n");
					return 0;
				}
				*(error->values+(i++)) = temp;
			}
			else {
				*(error->values+(i++)) = 0;
			}
        }
		if (input[strlen(input) - 1] == ',') {
            printf("Extraneous text after end of command\n");
            return 0;
        }
        if (i == 0) {
            printf("Missing argument\n");
            return 0;
        }
    }
	
	else if (strcmp(error->command, "add_mat") == 0 ||
			strcmp(error->command, "sub_mat") == 0 ||
			strcmp(error->command, "mul_mat") == 0) {
        while (i < 3) {
            *(error->matrixs + i) = name_of_matrix(trim(strtok(NULL, ",")));
            if (!*(error->matrixs + (i++))) return 0;
        }

        if (((strtok(NULL, ",")) != NULL)||(input[strlen(input) - 1] == ',')) {
            printf("Extraneous text after end of command\n");
            return 0;
        }
    } 
	
	else if (strcmp(error->command, "mul_scalar") == 0) {
        *(error->matrixs) = name_of_matrix(trim(strtok(NULL, ",")));
        if (!*(error->matrixs)) return 0;

        if (sscanf(strtok(NULL, ","), "%f %c", &temp, &extra) != 1) {
            printf("Argument is not a scalar\n");
            return 0;
        }
        *(error->values) = temp;

        *(error->matrixs + 1) = name_of_matrix(trim(strtok(NULL, ",")));
        if (!*(error->matrixs + 1)) return 0;

        if (((strtok(NULL, ",")) != NULL)||(input[strlen(input) - 1] == ',')) {
            printf("Extraneous text after end of command\n");
            return 0;
        }

    }
	
	else if (strcmp(error->command, "trans_mat") == 0) {
        while (i < 2) {
            *(error->matrixs + i) = name_of_matrix(trim(strtok(NULL, ",")));
            if (!*(error->matrixs + (i++))) return 0;
        }
        if (((strtok(NULL, ",")) != NULL)||(input[strlen(input) - 1] == ',')) {
            printf("Extraneous text after end of command\n");
            return 0;
        }

    }
	
	else if (strcmp(error->command, "print_mat") == 0) {
        *(error->matrixs) = name_of_matrix(strtok(NULL, ","));
        if (!*(error->matrixs)) return 0;

        if (((strtok(NULL, ",")) != NULL)||(input[strlen(input) - 1] == ',')) {
            printf("Extraneous text after end of command\n");
            return 0;
        }

    }
	
	else if (strcmp(error->command, "stop") == 0) {
        if ((strtok(NULL, ",")) != NULL) {
            printf("Extraneous text after end of command\n");
            return 0;
        }
    } else {
		if (strstr(error->command, ",") != 0) {
			printf("Illegal comma\n");
			return 0;
		}
        printf("Undefined command name\n");
        return 0;
    }

    return 1;
}