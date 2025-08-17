#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "structs.h"
#include "assembler.h"
#include "secondPass.h"

/*Add entry list*/
int add_ent_label(Ent **head, char *name) {
	Ent *ptr = *head;
	Ent *new_label = (Ent *)malloc(sizeof(Ent));


	strcpy(new_label->name, name);
	new_label->next = NULL;

	if (*head == NULL) {
		*head = new_label;
	} else {
		ptr = *head;

		while (ptr->next != NULL) {
			if (strcmp(ptr->name, name) == 0) {
				free(new_label);
				return 1;
			}
			ptr = ptr->next;
		}
		if (strcmp(ptr->name, name) == 0) {
			free(new_label);
			return 1;
		}
		ptr->next = new_label;
	}
	return 0;
}

/*Add label list*/
int add_label(Label **head, char *name, int address, char *type, char *attribute) {
	Label *ptr;
	Label *new_label = (Label *)malloc(sizeof(Label));


	strcpy(new_label->name, name);
	new_label->address = address;
	strcpy(new_label->type, type);
	strcpy(new_label->attribute, attribute);
	new_label->next = NULL;

	if (*head == NULL) {
		*head = new_label;
	} else {
		ptr = *head;

		while (ptr->next != NULL) {
			if (strcmp(ptr->name, name) == 0) {
				free(new_label);
				return 1;
			}
			ptr = ptr->next;
		}
		if (strcmp(ptr->name, name) == 0) {
			free(new_label);
			return 1;
		}
		ptr->next = new_label;
	}
	return 0;
}

/*Add bin libe list*/
void Bin_line(BinCode **head, char *name) {
	BinCode *new_name = (BinCode *)malloc(sizeof(BinCode));
	strcpy(new_name->name, name);
	new_name->next = NULL;

	if (*head == NULL) {
		*head = new_name;
	} else {
		BinCode *ptr = *head;
		while (ptr->next != NULL) {
			ptr = ptr->next;
		}
		ptr->next = new_name;
	}
}

/*Remove spaces*/
void remove_spaces(char *str) {
	char *src = str, *dst = str;
	while (*src) {
		if (*src != ' ' && *src != '\t' && *src != '\n' && *src != '\r') {
			*dst++ = *src;
		}
		src++;
	}
	*dst = '\0';
}

/*Check and returnd the number of operands in each command*/
char opcode(char *command, char *name) {/*the first line with the command*/
	name[0] = '\0';
	if (strcmp(command, "mov")==0) {strcat(name, "0000"); return 2;}
	if (strcmp(command, "cmp")==0) {strcat(name, "0001"); return 2;}
	if (strcmp(command, "add")==0) {strcat(name, "0010"); return 2;}
	if (strcmp(command, "sub")==0) {strcat(name, "0011"); return 2;}
	if (strcmp(command, "lea")==0) {strcat(name, "0100"); return 2;}
	if (strcmp(command, "clr")==0) {strcat(name, "0101"); return 1;}
	if (strcmp(command, "not")==0) {strcat(name, "0110"); return 1;}
	if (strcmp(command, "inc")==0) {strcat(name, "0111"); return 1;}
	if (strcmp(command, "dec")==0) {strcat(name, "1000"); return 1;}
	if (strcmp(command, "jmp")==0) {strcat(name, "1001"); return 1;}
	if (strcmp(command, "bne")==0) {strcat(name, "1010"); return 1;}
	if (strcmp(command, "jsr")==0) {strcat(name, "1011"); return 1;}
	if (strcmp(command, "red")==0) {strcat(name, "1100"); return 1;}
	if (strcmp(command, "prn")==0) {strcat(name, "1101"); return 1;}
	if (strcmp(command, "rts")==0) {strcat(name, "1110"); return 0;}
	if (strcmp(command, "stop")==0) {strcat(name, "1111"); return 0;}
	return -1;
}

/*Compare the decimal string to binary string*/
void dec_to_bin(char *decimal_str, char *binary_str, int bits) {
	int num = atoi(decimal_str);
	int i;
	if (num < 0) {
		num = (1 << bits) + num;
	}

	for (i = bits - 1; i >= 0; i--) {
		binary_str[i] = (num & 1) ? '1' : '0';
		num >>= 1;
	}

	binary_str[bits] = '\0';
}

/**Check_digit - Validates an operand and reports errors based on its type.
 *
 * @param operand The operand string to check.
 * @param i Type of operand (0=immediate, 2=matrix, 3=register, 4=matrix numbers, 5=comma list, 6=label).
 * @param lineNumber Pointer to current line number (for error messages).
 * @param error Pointer to error flag (set to 1 if an error occurs).
 * @return 0 if valid, 1 if an error is found.
 */
int check_digit(char *operand, int i, int *lineNumber, int *error){
	if (i==0) {
		int pointer = 1;
		if (operand[1] == '\0') {
			printf("Error: in line %d - no number appears after #\n", *lineNumber);
			return 1;
		}
		while (operand[pointer] != '\0') {
			if (operand[pointer] < '0' || operand[pointer] > '9') {
				if ((pointer != 1) || ((pointer == 1) && (operand[1] != '-' && operand[1] != '+'))) {
					printf("Error: in line %d - invalid number\n", *lineNumber);
					return 1;
				}
			}
			pointer++;
		}
		return 0;
	}

	else if (i==2){
		int j = 0;
		char *pointer = operand;
		for (j=0;j<2;j++){
			pointer = strchr(pointer, '[');
			if (pointer == NULL || *(pointer+3) != ']'){
				printf("Error: in line %d - The matrix is incorrect\n", *lineNumber);
			return 1;
			}
			if (*(pointer+1) != 'r' || *(pointer+2) < '0' || *(pointer+2) > '7'){
				printf("Error: in line %d - The operand is incorrect\n", *lineNumber);
			return 1;
			}
		pointer++;
		}
		if (operand[0] == '[' || operand[strlen(operand)-1] !=']') {
			printf("Error: in line %d - The label is not in the correct location or does not exist.\n", *lineNumber);
		}
	return 0;
	}

	else if (i==3){
		if (strncmp(operand, "r", 1) != 0 || strlen(operand) != 2 || operand[1] < '0' || operand[1] > '7') {
			printf("Error: in line %d - The operand is incorrect\n", *lineNumber);
		return 1;
		}
	}

	else if (i==4){
		int j = 0, ptr;
		int numbers[2] = {0,0};
		char number[sizeof(int)];
		char *pointer = operand;
		char *close;
		for (j=0;j<2;j++){
			ptr = 0;
			pointer = strchr(pointer, '[');
			close = strchr(pointer, ']');
			if (pointer == NULL || close == NULL) {
				printf("Error: in line %d - The matrix is incorrect\n", *lineNumber);
				return 1;
			}
			pointer++;
			while (*pointer != ']' && *pointer != '\0' && *pointer != '\n' && *pointer != '\r'){
				if (*pointer > '9' || *pointer < '0'){
					printf("Error: in line %d - The number is incorrect\n", *lineNumber);
				return 1;
				}
			number[ptr] = *pointer;
			pointer++;
			ptr++;
			}
			numbers[j] = atoi(number);
			number[0] = '\0';
			pointer = close + 1;
		}
	return numbers[0] * numbers[1];
	}

	else if (i==5){
		char *pointer = operand;
		while (*pointer != '\n' && *pointer != '\0' && *pointer != '\r'){
			if (*pointer == ','){
				if (*(pointer+1) == ','){
					printf("Error: in line %d - Double comma\n", *lineNumber);
					*error = 1;
				}
			}
			else if (*pointer < '0' || *pointer > '9') {
				if ((*(pointer-1) != ',') || ((*(pointer-1) == ',') && (*pointer != '-' && *pointer != '+'))) {
					printf("Error: in line %d - The number is incorrect\n", *lineNumber);
				*error = 1;
				}
			}
		pointer ++;
		}
		if (operand[0] == ',' || *(pointer-1) == ','){
			printf("Error: in line %d - The comma is not placed correctly\n", *lineNumber);
		*error = 1;
		}
	if (*error == 1){return 1;}
	return 0;
	}

	else if (i==6){
		int len = strlen(operand);
		char *pointer = operand;
		if ((len > 32) || (len == 30 && operand[len-1] != '\n')) {
            printf("Error: in line %d - Label too long (more than 30 characters)\n", *lineNumber);
			*error = 1;
		}
		if (check_instruction(operand) == 1){
			printf("Error: in line %d - The label is called an instruction or guideline\n", *lineNumber);
			*error = 1;
		}
		if (isdigit(operand[0])){
			printf("Error: in line %d - There is number in first Label\n", *lineNumber);
			pointer++;
			*error = 1;
		}
		while (*pointer != '\0'){
			if ((*pointer < 'A' || *pointer > 'Z') && (*pointer < 'a' || *pointer > 'z') && (*pointer < '0' || *pointer > '9')){
				printf("Error: in line %d - The label is incorrect\n", *lineNumber);
				*error = 1;
			}
		pointer++;
		}
		if (*error == 1){return 1;}
		return 0;
	}
return 0;
}

/*Check if there are current number of operands in each command*/
int check_Addressing_Method(char *command_name, int operandNumber, int *mode){
	if ((operandNumber == 1) && (strcmp(command_name, "lea") == 0) && (*mode == 0 || *mode == 3)) {return 1;}
	else if ((operandNumber == 2) && !(((strcmp(command_name, "cmp") == 0) || (strcmp(command_name, "prn") == 0) || (strcmp(command_name, "rts") == 0) || (strcmp(command_name, "stop") == 0))) && (*mode == 0)) {return 1;}
return 0;
}

/*Check if there is an extra or no label in entry or extern command */
int ex_en(assembler_context *content) {
	char operands_copy[82];
	char *token;
	strcpy(operands_copy, content->totalLine);
	token = strtok(operands_copy, " \t\0");
	remove_spaces(operands_copy);
	if (token == NULL) {
		printf("Error: in line %d - No label defined.\n", content->lineNumber);
		content->error = 1;
		return 1;
	}

	token = strtok(NULL, " \t\0");
	if (token != NULL) {
		printf("Error: in line %d - Extra label defined.\n", content->lineNumber);
		content->error = 1;
		return 1;
	}
	remove_spaces(content->totalLine);
	return 0;
}

/*Free the label list*/
void free_label_list(Label *symbol_table_head) {
	Label *ptr = symbol_table_head;
	while (ptr) {
		Label *temp = ptr;
		ptr = ptr->next;
		free(temp);
	}
}

/*Print the binary list to the object file*/
void print_bin_list(assembler_context *content, FILE *out) {
	BinCode *ptr = content->symbol_bin_code;
	int i = 100;
	char number[9];
	char name[82];
	char IC[9], DC[9];
	sprintf(IC, "%d", content->IC);
	sprintf(DC, "%d", content->DC);
	dec_to_bin(IC,IC,8);
	dec_to_bin(DC,DC,8);
	base_four(IC, 8);
	base_four(DC, 8);
	fprintf(out, "%s \t %s\n", IC, DC);
	while (ptr != NULL) {
		strcpy(name, ptr->name);
		sprintf(number, "%d", i);
		dec_to_bin(number,number,8);
		base_four(number, 8);
		base_four(name, 10);
		fprintf(out, "%s \t %s\n", number, name);
		i++;
		ptr = ptr->next;
	}
}

/*Print the entry commands to ent file*/
void print_ent_list(Label *symbol_table_head, FILE *out) {
	Label *ptr = symbol_table_head;
	char line[11];
	while (ptr != NULL) {
		if (strcmp(ptr->attribute, "entry") == 0) {
			sprintf(line, "%d", ptr->address+100);
			dec_to_bin(line, line, 8);
			base_four(line, 8);
			fprintf(out, "%s \t %s\n", ptr->name, line);
		}
		ptr = ptr->next;
	}
}

/*Free binary list*/
void free_bin_list(BinCode *head) {
	BinCode *ptr = head;
	while (ptr) {
		BinCode *temp = ptr;
		ptr = ptr->next;
		free(temp);
	}
}

/*Free entry list*/
void free_ent_list(Ent *head) {
	Ent *ptr = head;
	while (ptr) {
		Ent *temp = ptr;
		ptr = ptr->next;
		free(temp);
	}
}