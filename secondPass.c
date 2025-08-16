#include <stdio.h>
#include <string.h>
#include "assembler.h"
#include "firstPass.h"
#include "util.h"
#include "secondPass.h"

int second_pass(char *filename, Label **symbol_table_head, BinCode **symbol_bin_code, Ent **symbol_ent) {

	FILE /**in = NULL, */*out = NULL;
	char *output_name = file_extension(filename, ".txt");
	char *input_name = file_extension(filename, ".am");
	char temp[11];
	Label *ptr1 = *symbol_table_head;
	BinCode *ptr2 = *symbol_bin_code;
	Ent *ptr3 = *symbol_ent;
	int lineNumber = 1;
	int error = 0;

	/*in = fopen(input_name, "r");*/
	out = fopen(output_name, "w");

	while (ptr3 != NULL) {
		while (ptr1 != NULL) {
			if (strcmp(ptr3->name, ptr1->name) == 0) {
				if (strcmp(ptr1->type,"extern") == 0) {
					printf("Label \"%s\" already exists as internal/external.\n", ptr3->name);
					error = 1;
				}
				else {
					strcpy(ptr1->attribute,"entry");
				}
				break;
			}
			ptr1 = ptr1->next;
		}
		if (ptr1 == NULL) {
			printf("Label \"%s\" is not found \n", ptr3->name);
			error = 1;
		}
		ptr1 = *symbol_table_head;
		ptr3 = ptr3->next;
	}

	ptr1 = *symbol_table_head;

	while (ptr2 != NULL) {
		if (ptr2->name[0] != '0' && ptr2->name[0] != '1') {
			while (ptr1){
				if (strcmp(ptr2->name, ptr1->name) == 0) {
					sprintf(temp, "%d", ptr1->address+100);
					if (strcmp(ptr1->type, "extern") == 0) {
						dec_to_bin("0", ptr2->name, 8);
						strcat(ptr2->name,"-01");
					}
					else{
						dec_to_bin(temp, ptr2->name, 8);
						strcat(ptr2->name,"-10");
					}
					break;
				}
				ptr1 = ptr1->next;
			}
			if (!ptr1){ printf("Error: In line %d - There is no label named \"%s\".\n", lineNumber, ptr2->name);}
			ptr1 = *symbol_table_head;
		}
		ptr2 = ptr2->next;
	}
	if (error > 0) {
		printf("The file %s was closed due to errors in the second pass.\n", input_name);
		return 1;
	}
	printf("Second pass completed successfully\n");
	print_label_list(*symbol_table_head,out);
	print_bin_list(*symbol_bin_code,out);
	print_ent_list(*symbol_ent,out);
	free_label_list(*symbol_table_head);
	free_bin_list(*symbol_bin_code);
	free_ent_list(*symbol_ent);
	fclose(out);
	return 0;
}