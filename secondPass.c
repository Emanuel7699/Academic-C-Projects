#include <stdio.h>
#include <string.h>
#include "structs.h"
#include "assembler.h"
#include "util.h"
#include "secondPass.h"

/**Second_pass - Performs the second pass of the assembler.
 *
 * @param content A struct containing all the assembler data, symbol tables, and counters.
 * @return 0 if the second pass finished successfully, 1 if there were errors.
 */
int second_pass(assembler_context *content) {

	FILE *ent = NULL, *ext = NULL;
	char *output_ob_name = file_extension(content->filename, ".ob");
	char *output_ent_name = file_extension(content->filename, ".ent");
	char *output_ext_name = file_extension(content->filename, ".ext");
	char *input_name = file_extension(content->filename, ".am");
	char temp[11];
	char line[11];
	int ent_written = 0, ext_written = 0;
	Label *ptr1 = content->symbol_table_head;
	BinCode *ptr2 = content->symbol_bin_code;
	Ent *ptr3 = content->symbol_ent;
	content->error = 0;
	content->lineNumber = 100;

	content->in = fopen(input_name, "r");
	content->out = fopen(output_ob_name, "w");
	ent = fopen(output_ent_name, "w");
	ext = fopen(output_ext_name, "w");
	while (ptr3 != NULL) {
		while (ptr1 != NULL) {
			if (strcmp(ptr3->name, ptr1->name) == 0) {
				if (strcmp(ptr1->type,"extern") == 0) {
					printf("Label \"%s\" already exists as extern.\n", ptr3->name);
					content->error = 1;
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
			content->error = 1;
		}
		ptr1 = content->symbol_table_head;
		ptr3 = ptr3->next;
	}

	ptr1 = content->symbol_table_head;
	while (ptr2 != NULL) {
		if (ptr2->name[0] != '0' && ptr2->name[0] != '1') {
			while (ptr1){
				if (strcmp(ptr2->name, ptr1->name) == 0) {
					sprintf(temp, "%d", ptr1->address+100);
					if (strcmp(ptr1->type, "extern") == 0) {
						dec_to_bin("0", ptr2->name, 8);
						strcat(ptr2->name,"01");
						sprintf(line, "%d", content->lineNumber);
						dec_to_bin(line, line, 8);
						base_four(line, 8);
						fprintf(ext, "%s \t %s\n",ptr1->name,line);
						ext_written = 1;
					}
					else{
						dec_to_bin(temp, ptr2->name, 8);
						strcat(ptr2->name,"10");
						ent_written = 1;
					}
					break;
				}
				ptr1 = ptr1->next;
			}
			if (!ptr1) {
				printf("Error: There is no label named \"%s\".\n", ptr2->name);
				content->error = 1;
			}
			ptr1 = content->symbol_table_head;
		}
		(content->lineNumber)++;
	ptr2 = ptr2->next;
	}
	if (content->error > 0) {
		free_label_list(content->symbol_table_head);
		free_bin_list(content->symbol_bin_code);
		free_ent_list(content->symbol_ent);
		fclose(ext);
		fclose(ent);
		fclose(content->out);
		fclose(content->in);
		remove(output_ob_name);
		remove(output_ext_name);
		remove(output_ent_name);
		return 1;
	}

	print_bin_list(content, content->out);
	print_ent_list(content->symbol_table_head, ent);
	free_label_list(content->symbol_table_head);
	free_bin_list(content->symbol_bin_code);
	free_ent_list(content->symbol_ent);
	fclose(content->in);
	fclose(content->out);
	fclose(ext);
	fclose(ent);
	if (!ent_written) remove(output_ent_name);
	if (!ext_written) remove(output_ext_name);
	return 0;
}

/*Convert the number to base 4*/
void base_four(char *bin_line, int peak) {
	int i, j = 0;

	for (i = 0; i < peak; i += 2) {
		bin_line[j++] = 'a' + (bin_line[i] - '0') * 2 + (bin_line[i+1] - '0');
	}
	bin_line[j] = '\0';
}