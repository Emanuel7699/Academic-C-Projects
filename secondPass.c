#include <stdio.h>
#include <string.h>
#include "assembler.h"
#include "firstPass.h"
#include "util.h"
#include "secondPass.h"
int second_pass(char *filename, Label **symbol_table_head, BinCode **symbol_bin_code) {
	FILE *out = NULL;
	char *output_name = file_extension(filename, ".txt");
	char temp[32];
	Label *ptr1 = *symbol_table_head;
	BinCode *ptr2 = *symbol_bin_code;
	out = fopen(output_name, "w");

	while (ptr2 != NULL) {
		if (ptr2->name[0] != '0' && ptr2->name[0] != '1') {
			while (ptr1){
				if (strstr(ptr2->name, ptr1->name) != NULL) {
					sprintf(temp, "%d", ptr1->address+100);
					printf("%s----->%s\n",ptr2->name, ptr1->name);
					dec_to_bin(temp, ptr2->name, 8);
					strcat(ptr2->name,"-10");
					break;
				}
				ptr1 = ptr1->next;
			}

			if (!ptr1) {
				dec_to_bin("0", ptr2->name, 8);
				strcat(ptr2->name,"-01");
			}
			ptr1 = *symbol_table_head;
		}
		ptr2 = ptr2->next;
	}
	print_label_list(*symbol_table_head,out);
	print_bin_list(*symbol_bin_code,out);
	free_label_list(*symbol_table_head);
	free_bin_list(*symbol_bin_code);
	fclose(out);
	return 0;
}























/*int second_pass(char *filename, Label **symbol_table_head, BinCode **symbol_bin_code)
{
	char line[82], line_copy[82], total_line[82];
	char label_name[32];
	char command[31];
	int i = 0;
	char *temp;
	FILE *in = NULL, *out = NULL;
	char *input_name = file_extension(filename, ".am");
	char *output_name = file_extension(filename, ".txt");

	in = fopen(input_name, "r");
	out = fopen(output_name, "w");

	while (fgets(line, sizeof(line), in) != NULL) {
		i++;
		strcpy(line_copy, line);
		if (strncmp(line, ";", 1) == 0) {
			continue;
		}
		remove_spaces(line);
		if (line[0] == '\0') {
			continue;
		}
		if (strchr(line, ':')) {
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
		if ((strlen(total_line)) > 0 && check_guideline(command) == 0){
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
}*/

/*char check_line(BinCode **symbol_bin_code, Label **symbol_table_head, char *total_line) {
	char line_copy[82];
	char *operand1 = NULL, *operand2 = NULL;
	int mode;
	strcpy(line_copy, total_line);
	operand1 = strtok(line_copy, ",\t\n");
	operand2 = strtok(NULL, ", \t\n");
	if (operand1 != NULL) {
		mode = get_addressing_mode(operand1);
		if (mode == 1 || mode == 2) {
			if (mode == 1) {
				check_label(symbol_bin_code, symbol_table_head, operand1);
			}
			else {
				check_label(symbol_bin_code, symbol_table_head, strtok(operand1,"["));
			}
		}
	}
	if (operand2 != NULL) {
		mode = get_addressing_mode(operand2);
		if (mode == 1 || mode == 2) {
			if (mode == 1) {
				check_label(symbol_bin_code, symbol_table_head, operand2);
			}
			else {
				check_label(symbol_bin_code, symbol_table_head, strtok(operand2,"["));
			}
		}
	}
	return 0;
}*/

/*void check_label(BinCode **symbol_bin_code, Label **symbol_table_head, char *name) {
	char temp[32];
	Label *ptr1 = *symbol_table_head;
	BinCode *ptr2 = *symbol_bin_code;
	remove_spaces(name);
	printf("%s\n",name);
	while (ptr1 != NULL) {
		if (strcmp(name, ptr1->name) == 0) {
			while (ptr2 && strcmp(ptr2->name, "?") != 0) {
				ptr2 = ptr2->next;
			}
			if (ptr2) {
				sprintf(temp, "%d", ptr1->address+100);
				printf("%s----->%s\n",name, ptr1->name);
				dec_to_bin(temp, ptr2->name, 8);
				strcat(ptr2->name,"10");
			}
		return;
		}
		ptr1 = ptr1->next;
	}
	while (ptr2 && strcmp(ptr2->name, "?") != 0) {
			ptr2 = ptr2->next;
	}
	if (ptr2) {
		dec_to_bin("0", ptr2->name, 8);
		strcat(ptr2->name,"01");
	}
}*/

/*int check_guideline(char *line) {
	if (strcmp(line, ".data") == 0 ||
		strcmp(line, ".string") == 0 ||
		strcmp(line, ".mat") == 0 ||
		strcmp(line, ".extern") == 0 ||
		strcmp(line, ".entry") == 0) {
		return 1;
		}
	return  0;
}*/