#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assembler.h"
#include "firstPass.h"

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


void check_command(Label **head, char *label_name, char *command, int DC, int IC) {
	/*printf("%s\n",command);*/
	remove_spaces(command);
	if (strcmp(command, ".data") == 0 || strcmp(command, ".string") == 0 || strcmp(command, ".mat") == 0) {
		add_label(head, label_name, DC+IC, "data", "");
	}
	else if (strcmp(command, ".extern") == 0) {
		add_label(head, label_name, 0, "extern", "extern");
	}
	else if (num_of_operands(command) !=-1) {
		add_label(head, label_name, IC+DC, "code", "");
	}
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
			/*//////////////////////////////////////////////שליחה לקוד שימיר לבינארי//*/
			(*DC)++;
			token = strtok(NULL, ",\t\n");
		}
	}
	else if (strcmp(directive, ".string") == 0) {
		if (operands[0] == '\"') {
			int length = 0;
			for (i=1; operands[i] != '\"' && operands[i] != '\0'; i++) {
				/*////////////////////////////////////////////////////////////////שליחה לקוד שימיר תוי אסקי לבינארי///*/
				length++;
			}
			(*DC) += (length + 1);
			/*//////////////////////////////////////////////////////////,\0,מילוי אפסים////////*/
		}
	}
	else if (strcmp(directive, ".mat") == 0) {
		strcpy(operands_copy, operands);

		token = strtok(operands_copy, " \t\n");
		if (token != NULL) token = strtok(NULL, ",");
		while (token != NULL) {
			/*///////////////////////////////////////////////////////////////////////*/
			(*DC)++;
			token = strtok(NULL, ",\n");
		}
	}
}


int count_words_for_instruction(BinCode **symbol_bin_code,char *command_name, char *total_line) {
	char *name = (char *)malloc(11 * sizeof(char));
	int count = 1;
	int mode1 = -1, mode2 = -1;
	char *operand1 = NULL, *operand2 = NULL;
	char line_copy[82];
	name[0] = '\0';
	strcpy(line_copy, total_line);
	opcode(command_name,name);

	if (strcmp(name,"1110") ==0 || strcmp(name,"1111") ==0) {
		strcat(name, "000000");
		Bin_line(symbol_bin_code, name);
	}

	if (total_line == NULL || strlen(total_line) == 0) {
		return count;
	}
	if (num_of_operands(command_name) == 1) {
		operand1 = NULL;
		operand2 = strtok(line_copy, "\n");
	}
	else if (num_of_operands(command_name) == 2) {
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


		strcat(name, "00");
		Bin_line(symbol_bin_code,name);
		check_bin(symbol_bin_code,operand1,operand2);
		free(name);
		printf("%d\t", count);
	}
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

void print_bin_list(BinCode *head, FILE *out) {
	BinCode *ptr = head;
	while (ptr != NULL) {
		fprintf(out, "%s\n", ptr->name);
		ptr = ptr->next;
	}
}

void free_bin_list(BinCode *head) {
	BinCode *ptr = head;
	while (ptr) {
		BinCode *temp = ptr;
		ptr = ptr->next;
		free(temp);
	}
}

char opcode(char *command, char *name) {/*the first line with the command*/
	if (strcmp(command, "mov")==0) {strcat(name, "0000");}
	if (strcmp(command, "cmp")==0) {strcat(name, "0001");}
	if (strcmp(command, "add")==0) {strcat(name, "0010");}
	if (strcmp(command, "sub")==0) {strcat(name, "0011");}
	if (strcmp(command, "lea")==0) {strcat(name, "0100");}
	if (strcmp(command, "clr")==0) {strcat(name, "0101");}
	if (strcmp(command, "not")==0) {strcat(name, "0110");}
	if (strcmp(command, "inc")==0) {strcat(name, "0111");}
	if (strcmp(command, "dec")==0) {strcat(name, "1000");}
	if (strcmp(command, "jmp")==0) {strcat(name, "1001");}
	if (strcmp(command, "bne")==0) {strcat(name, "1010");}
	if (strcmp(command, "jsr")==0) {strcat(name, "1011");}
	if (strcmp(command, "red")==0) {strcat(name, "1100");}
	if (strcmp(command, "prn")==0) {strcat(name, "1101");}
	if (strcmp(command, "rts")==0) {strcat(name, "1110");}
	if (strcmp(command, "stop")==0) {strcat(name, "1111");}
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