#include <stdio.h>
#include <string.h>
#include "assembler.h"
#include "firstPass.h"
#include "util.h"
#include "secondPass.h"

int second_pass(char *filename, Label **symbol_table_head, BinCode **symbol_bin_code) {
	char line[82], line_copy[82], total_line[82];
	char label_name[32];
	char command[31];
	int i = 0;
	char *temp;
	FILE *in = NULL, *out = NULL;
	char *input_name = file_extension(filename, ".am");
	char *output_name = file_extension(filename, ".txt");

	in = fopen(input_name, "r");
	out = fopen(output_name, "w+");

	while (fgets(line, sizeof(line), in) != NULL) {
		i++;
		strcpy(line_copy, line);
		if (strncmp(line, ";", 1) == 0) {/*if there is a comment line*/
			continue;
		}
		remove_spaces(line);
		if (line[0] == '\0') {
			continue;
		}
		if (strchr(line, ':')) {/*if there is a label*/
			strcpy(label_name, strtok(line_copy, ":"));

			strcpy(command, strtok(NULL, " \t\n"));
		}
		else {
			strcpy(command, strtok(line_copy, " \t\n"));
		}

		temp = strtok(NULL, "\n");
		if (temp != NULL) {
			strcpy(total_line, temp);
		}
		else {
			total_line[0] = '\0';
		}
		if (strlen(total_line) > 0){
			check_line(symbol_bin_code, symbol_table_head,total_line);
		}
	}
	print_label_list(*symbol_table_head,out);
	print_bin_list(*symbol_bin_code,out);
	free_label_list(*symbol_table_head);
	free_bin_list(*symbol_bin_code);
	fclose(in);
	fclose(out);
	return 0;
}

char check_line(BinCode **symbol_bin_code, Label **symbol_table_head, char *total_line) {
	printf("ננננננ");
	char line_copy[82];
	char *operand1 = NULL, *operand2 = NULL;
	int mode;
	printf("%s\n",total_line);
	strcpy(line_copy, total_line);
	operand1 = strtok(line_copy, ",\t\n");
	operand2 = strtok(NULL, ", \t\n");
	printf("%s\n",operand1);
	printf("%s\n",operand2);
	if (operand1 != NULL) {
		mode = get_addressing_mode(operand1);
		if (mode == 1 || mode == 2) {
			printf("'''''''''אאאאאאאאאאאאאאאאא'''''");
			check_label(symbol_bin_code, symbol_table_head, operand1);
		}
	}
	if (operand2 != NULL) {
		mode = get_addressing_mode(operand2);
		if (mode == 1 || mode == 2) {
			printf("''''''''''''''");
			check_label(symbol_bin_code, symbol_table_head, operand2);
		}
	}
	return 1;
}

void check_label(BinCode **symbol_bin_code, Label **symbol_table_head, char *name) {
	char temp[32];
	Label *ptr1 = *symbol_table_head;
	BinCode *ptr2 = *symbol_bin_code;
	printf("aaaaaaaaaaaaaaa");
	while (ptr1 != NULL) {
		printf("bbbbbbbbbbb");
		if (strstr(name, ptr1->name) != NULL) {
			printf("ccccccccccc");
			while (strcmp(ptr2->name, "?") != 0) {
				ptr2 = ptr2->next;
			}
			printf("ddddddddddddd");
			sprintf(temp, "%d", ptr1->address);
			dec_to_bin(temp, ptr2->name, 8);
			return;
		}
		ptr1 = ptr1->next;
	}
}