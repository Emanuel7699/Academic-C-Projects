#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assembler.h"
#include "TEST.h"

int first_pass(char *filename) {
	int IC = 0, DC = 0;

	char line[82], line_copy[82];
	Label symbol_table[100];
	int label_count = 0;

	char *command;
	char *operand1;
	char *operand2;

	FILE *in = NULL;
	char *amfile = file_extension(filename,".am");
	in = fopen(amfile, "r");

	while (fgets(line, sizeof(line), in) != NULL) {
		strcpy(line_copy, line);
		if (strncmp(line, ";", 1) == 0) {/*if there is a comment line*/
			continue;
		}

		if (strchr(line, ':')) {/*if there is a label*/
			if (strcmp(line,".entry")) continue;
			char *label_name = strtok(line_copy, ":");
			char *after_label = strtok(NULL, "");
			strcpy(line_copy, after_label);
			add_label(symbol_table, &label_count, label_name, IC, "code", "");
		}

		command = strtok(line_copy, " \t\n");
		operand1 = strtok(NULL, ", \t\n");
		operand2 = strtok(NULL, ", \t\n");
		fprintf(out, "%d\n", opcode(command));
		if (strcmp(command, ".data") == 0 || strcmp(command, ".string") == 0 || strcmp(command, ".mat") == 0) {
			process_data_directive(command, operand1, &DC);

		}
		else if (strcmp(command, ".extern") == 0) {
			add_label(symbol_table, &label_count, operand1, 0, "external", "");
		}
		else if (strcmp(command, ".entry") == 0) {
			continue;
		}
		else {
			int words = count_words_for_instruction(command, operand1, operand2);
			fprintf(out, "%d\n", opcode(command));
			IC += words;
		}
	}
	return 0;
}

void add_label(Label labels[], int *num_labels, char *name, int address, char *type, char *attribute) {
	strcpy(labels[*num_labels].name, name);
	labels[*num_labels].address = address;
	strcpy(labels[*num_labels].type, type);
	strcpy(labels[*num_labels].attribute, attribute);
	(*num_labels)++;
}

int get_addressing_mode(char *operand) {
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

int count_words_for_instruction(char *command_name, char *operand1, char *operand2) {
	int count = 1;
	int mode1 = -1, mode2 = -1;

	if (operand1 != NULL) {
		mode1 = get_addressing_mode(operand1);
		if (mode1 == 2)
			count += 2;
		else if (mode1 == 3)
			count += 1;
		else
			count += 1;
	}
	if (operand2 != NULL) {
		mode2 = get_addressing_mode(operand2);
		if (mode2 == 2)
			count += 2;
		else if (mode2 == 3 && mode1 == 3)
			count -= 1;
		else
			count += 1;
	}
	return count;
}

void process_data_directive(char *directive, char *operands, int *DC) {
	char operands_copy[82];
	char *token;
	int i;
	if (strcmp(directive, ".data") == 0) {

		strcpy(operands_copy, operands);

		token = strtok(operands_copy, ", \t\n");
		while (token != NULL) {
			(*DC)++;
			token = strtok(NULL, ", \t\n");
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

		token = strtok(operands_copy, ", \t\n");
		while (token != NULL) {
			(*DC)++;
			token = strtok(NULL, ", \t\n");
		}
	}
	return;
}

int count_words_for_instruction(char *command_name, char *total_line) {

	int count = 1;
	int mode1 = -1, mode2 = -1;

	if (num_of_operands(command_name) == "01") {
		operand1 = NULL;
		operand2 = strtok(NULL, "\n");
	}
	else if (num_of_operands(command_name) == "11") {
		operand1 = strtok(total_line, ",");
		operand2 = strtok(NULL, "\n");
	}
	if (operand1 != NULL) {
		mode1 = get_addressing_mode(operand1);
		if (mode1 == 2)
			count += 2;
		else if (mode1 == 3)
			count += 1;
		else
			count += 1;
	}
	if (operand2 != NULL) {
		mode2 = get_addressing_mode(operand2);
		if (mode2 == 2)
			count += 2;
		else if (mode2 == 3 && mode1 == 3)
			count -= 1;
		else
			count += 1;
	}
	return count;
}