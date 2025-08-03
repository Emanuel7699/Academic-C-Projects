#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assembler.h"
#include "firstPass.h"
#include "util.h"

int first_pass(char *filename) {
	int IC = 0, DC = 0;

	Label *symbol_table_head = NULL;
	BinCode *symbol_bin_code = NULL;
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
		remove_spaces(line);
		if (line[0] == '\0') {
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
			process_data_directive(&symbol_bin_code, command, total_line, &DC);
		}
		else if (strcmp(command, ".extern") == 0) {
			continue;
		}
		else if (strcmp(command, ".entry") == 0) {
			continue;
		}
		else {
			int words = count_words_for_instruction(&symbol_bin_code, command, total_line);
			IC += words;
		}
	}

	print_label_list(symbol_table_head,out);
	print_bin_list(symbol_bin_code,out);
	free_label_list(symbol_table_head);
	free_bin_list(symbol_bin_code);
	fclose(in);
	fclose(out);
	return 0;
}

void check_command(Label **head, char *label_name, char *command, int DC, int IC) {
	char temp[10];
	remove_spaces(command);
	if (strcmp(command, ".data") == 0 || strcmp(command, ".string") == 0 || strcmp(command, ".mat") == 0) {
		add_label(head, label_name, DC+IC, "data", "");
	}
	else if (strcmp(command, ".extern") == 0) {
		add_label(head, label_name, 0, "extern", "extern");
	}
	else if (opcode(command,temp) !=-1) {
		add_label(head, label_name, IC+DC, "code", "");
	}
}

void process_data_directive(BinCode **symbol_bin_code, char *directive, char *operands, int *DC) {
	char *name = (char *)malloc(11 * sizeof(char));
	char operands_copy[82];
	char ascii[12];
	char *token;
	int i;
	name[0] = '\0';
	if (strcmp(directive, ".data") == 0) {
		strcpy(operands_copy, operands);

		token = strtok(operands_copy, ",\t\n");
		while (token != NULL) {
			dec_to_bin(token,name,10);
			Bin_line(symbol_bin_code, name);
			(*DC)++;
			token = strtok(NULL, ",\t\n");
		}
	}
	else if (strcmp(directive, ".string") == 0) {
		if (operands[0] == '\"') {
			int length = 0;
			for (i=1; operands[i] != '\"' && operands[i] != '\0'; i++) {
				sprintf(ascii, "%d", (int)operands[i]);
				dec_to_bin(ascii,name,10);
				Bin_line(symbol_bin_code, name);
				length++;
			}
			(*DC) += (length + 1);
			strcpy(name,"0000000000");
			Bin_line(symbol_bin_code, name);
		}
	}
	else if (strcmp(directive, ".mat") == 0) {
		strcpy(operands_copy, operands);

		token = strtok(operands_copy, " \t\n");
		if (token != NULL) token = strtok(NULL, ",");
		while (token != NULL) {
			dec_to_bin(token,name,10);
			Bin_line(symbol_bin_code, name);
			(*DC)++;
			token = strtok(NULL, ",\n");
		}
	}
	free(name);
}

int count_words_for_instruction(BinCode **symbol_bin_code,char *command_name, char *total_line) {
	char *name = (char *)malloc(11 * sizeof(char));
	int count = 1;
	int type = -1, mode1 = -1, mode2 = -1;
	char *operand1 = NULL, *operand2 = NULL;
	char line_copy[82];
	strcpy(line_copy, total_line);
	type = opcode(command_name,name);

	if (strcmp(name,"1110") ==0 || strcmp(name,"1111") ==0) {
		strcat(name, "000000");
		Bin_line(symbol_bin_code, name);
	}

	if (total_line == NULL || strlen(total_line) == 0) {
		return count;
	}
	if (type == 1) {
		operand1 = NULL;
		operand2 = strtok(line_copy, "\n");
	}
	else if (type == 2) {
		operand1 = strtok(line_copy, ",");
		operand2 = strtok(NULL, "\n");
	}



	if (operand1 != NULL) {
		remove_spaces(operand1);
		mode1 = get_addressing_mode(operand1);
		if (mode1 == 0){
			strcat(name, "00");
			count += 1;
		}
		else if (mode1 == 1){
			strcat(name, "01");
			count += 1;
		}
		else if (mode1 == 2) {
			strcat(name, "10");
			count += 2;
		}
		else if (mode1 == 3) {
			strcat(name, "11");
			count += 1;
		}
	}
	else {
		strcat(name, "00");
	}


	if (operand2 != NULL) {
		remove_spaces(operand2);
		mode2 = get_addressing_mode(operand2);
		if (mode2 == 0) {
			strcat(name, "00");
			count += 1;
		}
		else if (mode2 == 1) {
			strcat(name, "01");
			count += 1;
		}
		else if (mode2 == 2){
			strcat(name, "10");
			count += 2;
		}
		else if (mode2 == 3) {
			if (mode2 == 3 && mode1 == 3) {
				strcat(name, "11");
				count += 0;
			}
			else {
				strcat(name, "11");
				count += 1;
			}
		}
		else {
			strcat(name, "00");
		}
	}
	strcat(name, "00");
	Bin_line(symbol_bin_code,name);
	check_bin(symbol_bin_code,operand1,operand2);
	printf("%d\t", count);
	free(name);
	return count;
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

char check_bin(BinCode **symbol_bin_code, char *operand1, char *operand2) {
	char *temp = (char *)malloc(11 * sizeof(char));
	char *name = (char *)malloc(11 * sizeof(char));
	char *name1 = (char *)malloc(11 * sizeof(char));
	int mode1 = -1, mode2 = -1;
	char *r1,*r2;
	temp[0] = '\0';
	name[0] = '\0';
	name1[0] = '\0';

	if (operand1 != NULL) {
		mode1 = get_addressing_mode(operand1);
		if (mode1 == 0) {
			dec_to_bin(operand1+1, name, 8);
			strcat(name,"00");
			Bin_line(symbol_bin_code,name);
		}
		else if (mode1 == 1) {
			strcpy(name,"?");
			Bin_line(symbol_bin_code,name);
		}
		else if (mode1 == 2) {
			strcpy(name,"?");
			r1 = strchr(operand1, 'r');
			dec_to_bin(r1+1, temp, 4);
			strcat(name1, temp);
			r2 = strchr(r1+1, 'r');
			dec_to_bin(r2+1, temp, 4);
			strcat(name1, temp);
			strcat(name1,"00");
			Bin_line(symbol_bin_code,name);
			Bin_line(symbol_bin_code,name1);
		}
		else if (mode1 == 3 && get_addressing_mode(operand2) != 3) {
			dec_to_bin(operand1+1, name, 4);
			strcat(name,"000000");
			Bin_line(symbol_bin_code,name);
		}
	}

	temp[0] = '\0';
	name[0] = '\0';
	name1[0] = '\0';
	if (operand2 != NULL) {
		mode2 = get_addressing_mode(operand2);
		if (mode2 == 0) {
			dec_to_bin(operand2+1, name, 8);
			strcat(name,"00");
			Bin_line(symbol_bin_code,name);
		}
		else if (mode2 == 1) {
			strcpy(name,"?");
			Bin_line(symbol_bin_code,name);
		}
		else if (mode2 == 2) {
			strcpy(name,"?");
			r1 = strchr(operand2, 'r');
			dec_to_bin(r1+1, temp, 4);
			strcat(name1, temp);
			r2 = strchr(r1+1, 'r');
			dec_to_bin(r2+1, temp, 4);
			strcat(name1, temp);
			strcat(name1,"00");
			Bin_line(symbol_bin_code,name);
			Bin_line(symbol_bin_code,name1);
		}
		else if (mode2 == 3) {
			if (mode2 == 3 && mode1 == 3) {
				dec_to_bin(operand1+1, name, 4);
				dec_to_bin(operand2+1, temp, 4);
				strcat(name,temp);
				strcat(name,"00");
				Bin_line(symbol_bin_code,name);
			}
			else {
				dec_to_bin(operand2+1, temp, 4);
				strcat(name, "0000");
				strcat(name, temp);
				strcat(name,"00");
				Bin_line(symbol_bin_code,name);
			}
		}
	}
	free(temp);
	free(name);
	free(name1);
	return 1;
}