#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assembler.h"
#include "firstPass.h"

int first_pass(char *filename) {
	int IC = 0, DC = 0;

	Label *symbol_table_head = NULL;
	char line[82], line_copy[82], total_line[82];
	char label_name[32];
	char command[31];
	char *temp;

	FILE *in = NULL, *out = NULL;

	char *input_name = file_extension(filename, ".am");
	char *output_name = file_extension(filename, ".ob");

	in = fopen(input_name, "r");
	out = fopen(output_name, "w");

	while (fgets(line, sizeof(line), in) != NULL) {

		strcpy(line_copy, line);
		if (strncmp(line, ";", 1) == 0) {/*if there is a comment line*/
			continue;
		}

		if (strchr(line, ':')) {/*if there is a label*/

			if (strstr(line,".entry")) continue;
			strcpy(label_name, strtok(line_copy, ":"));
			strcpy(command, strtok(NULL, " \t\n"));
			printf("%s\t", command);
			check_command(&symbol_table_head, label_name,  command, DC, IC);
		}
		else {
			strcpy(command, strtok(line_copy, " \t\n"));
			printf("%s\t", command);
		}
		temp = strtok(NULL, "\n");
		if (temp != NULL) {
			strcpy(total_line, temp);
		}
		else {
			total_line[0] = '\0';
		}
		printf("%s\n", total_line);

		if (strcmp(command, ".data") == 0 || strcmp(command, ".string") == 0 || strcmp(command, ".mat") == 0) {
			process_data_directive(command, total_line, &DC);
		}
		else if (strcmp(command, ".extern") == 0) {
			continue;
		}
		else if (strcmp(command, ".entry") == 0) {
			continue;
		}
		else {
			int words = count_words_for_instruction(command, total_line);
			IC += words;
		}
	}

	print_label_list(symbol_table_head,out);
	free_label_list(symbol_table_head);
	fclose(in);
	fclose(out);
	return 0;
}


void add_label(Label **head, char *name, int address, char *type, char *attribute) {
	Label *new_label = (Label *)malloc(sizeof(Label));
	strcpy(new_label->name, name);
	new_label->address = address;
	strcpy(new_label->type, type);
	strcpy(new_label->attribute, attribute);
	new_label->next = NULL;

	if (*head == NULL) {
		*head = new_label;
	} else {
		Label *ptr = *head;
		while (ptr->next != NULL) {
			ptr = ptr->next;
		}
		ptr->next = new_label;
	}
}


void check_command(Label **head, char *label_name, char *command, int DC, int IC) {
	/*printf("%s\n",command);*/
	remove_spaces(command);
	if (strcmp(command, ".data") == 0 || strcmp(command, ".string") == 0 || strcmp(command, ".mat") == 0) {
		add_label(head, label_name, DC+IC, "data", "");
	}
	else if (strcmp(command, ".extern") == 0) {
		add_label(head, label_name, 0, "extern", "extern");
	}
	else if (!check_data(command)) {
		add_label(head, label_name, IC+DC, "code", "");
	}
}


int check_data(char *command) {
	if (strcmp(command, "mov") ||
		strcmp(command, "cmp") ||
		strcmp(command, "add") ||
		strcmp(command, "sub") ||
		strcmp(command, "lea") ||
		strcmp(command, "clr") ||
		strcmp(command, "not") ||
		strcmp(command, "inc") ||
		strcmp(command, "dec") ||
		strcmp(command, "jmp") ||
		strcmp(command, "bne") ||
		strcmp(command, "jsr") ||
		strcmp(command, "red") ||
		strcmp(command, "prn") ||
		strcmp(command, "rts") ||
		strcmp(command, "stop")) {
		return 0;
	}
	return 1;
}


int num_of_operands(char *command) {
	if (strcmp(command, "mov") == 0 ||
		strcmp(command, "cmp") == 0 ||
		strcmp(command, "add") == 0 ||
		strcmp(command, "sub") == 0 ||
		strcmp(command, "lea") == 0){
	return 2;
	}
	else if (strcmp(command, "clr") == 0 ||
		strcmp(command, "not") == 0 ||
		strcmp(command, "inc") == 0 ||
		strcmp(command, "dec") == 0 ||
		strcmp(command, "jmp") == 0 ||
		strcmp(command, "bne") == 0 ||
		strcmp(command, "jsr") == 0 ||
		strcmp(command, "red") == 0 ||
		strcmp(command, "prn") == 0){
	return 1;
	}
	else if (strcmp(command, "rts") == 0 ||
		strcmp(command, "stop") == 0){
	return 0;
	}
	fprintf(stderr, "Error: The command is undefined\n");
	return  -1;
}


void process_data_directive(char *directive, char *operands, int *DC) {
	char operands_copy[82];
	char *token;
	int i;
	if (strcmp(directive, ".data") == 0) {
		strcpy(operands_copy, operands);

		token = strtok(operands_copy, ",\t\n");
		while (token != NULL) {
			(*DC)++;
			token = strtok(NULL, ",\t\n");
		}
	}
	else if (strcmp(directive, ".string") == 0) {
		if (operands[0] == '\"') {
			int length = 0;
			for (i=1; operands[i] != '\"' && operands[i] != '\0'; i++) {
				length++;
			}
			(*DC) += (length + 1);
		}
	}
	else if (strcmp(directive, ".mat") == 0) {
		strcpy(operands_copy, operands);

		token = strtok(operands_copy, " \t\n");
		if (token != NULL) token = strtok(NULL, ",");
		while (token != NULL) {
			(*DC)++;
			token = strtok(NULL, ",\n");
		}
	}
}


int count_words_for_instruction(char *command_name, char *total_line) {
	/*printf("%s\n", command_name);*/

	int count = 1;
	int mode1 = -1, mode2 = -1;
	char *operand1 = NULL, *operand2 = NULL;

	if (total_line == NULL || strlen(total_line) == 0) {
		return count;
	}
	if (num_of_operands(command_name) == 1) {
		operand1 = NULL;
		operand2 = strtok(total_line, "\n");
	}
	else if (num_of_operands(command_name) == 2) {
		operand1 = strtok(total_line, ",");
		operand2 = strtok(NULL, "\n");
	}
	if (operand1 != NULL) {
		mode1 = get_addressing_mode(operand1);
		if (mode1 == 2) {
			count += 2;
		}
		else if (mode1 == 3) {
			count += 1;
		}
		else {
			count += 1;
		}
	}
	if (operand2 != NULL) {
		mode2 = get_addressing_mode(operand2);
		if (mode2 == 2){
			count += 2;
		}
		else if (mode2 == 3 && mode1 == 3) {
			count += 0;
		}
		else {
			count += 1;
		}
	}
	printf("%d\t", count);
	return count;
}


int get_addressing_mode(char *operand) {
	remove_spaces(operand);
	if (operand[0] == '#') {
		return 0;
	}
	if (strchr(operand, '[') != NULL && strchr(operand, ']') != NULL) {
		return 2;
	}
	if (strncmp(operand, "r", 1) == 0 && strlen(operand) == 2 && operand[1] >= '0' && operand[1] <= '7') {
		return 3;
	}
	return 1;
}


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

void free_label_list(Label *symbol_table_head) {
	Label *ptr = symbol_table_head;
	while (ptr) {
		Label *temp = ptr;
		ptr = ptr->next;
		free(temp);
	}
}

void print_label_list(Label *symbol_table_head, FILE *out) {
	Label *ptr = symbol_table_head;
	while (ptr != NULL) {
		fprintf(out, "%s %d %s %s\n",
			ptr->name,
			ptr->address + 100,
			ptr->type,
			ptr->attribute);
		ptr = ptr->next;
	}
}