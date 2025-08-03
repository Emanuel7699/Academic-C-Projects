#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "firstPass.h"

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
	fprintf(stderr, "Error: The command is undefined\n");
	return -1;
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