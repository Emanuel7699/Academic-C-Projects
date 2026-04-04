#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"
#include "assembler.h"
#include "firstPass.h"
#include "util.h"

/**First_pass - Performs the first pass of the assembler.
 *
 * It reads and builds the symbol table, and checks for errors.
 *
 * @param content a struct that holds all the variables used across functions.
 * @return 0 if the first pass was successful, 1 if there were errors.
 */
int first_pass(assembler_context *content) {
	FILE *temp_macro = NULL;
	char *temp;
	char *input_name = file_extension(content->filename, ".am");

	content->IC = 0;
	content->DC = 0;
	content->error = 0;
	content->lineNumber = 1;
	content->symbol_table_head = NULL;
	content->symbol_bin_code = NULL;
	content->symbol_ent = NULL;

	content->in = fopen(input_name, "r");
	temp_macro = fopen("temp_macro.am", "r");

	if (!content->in) {
		printf("Error opening input file");
		return 1;
	}
	if (!content->out) {
		printf("Error opening output file");
		fclose(content->in);
		return 1;
	}
	if (!temp_macro) {
		printf("Error: temp_macro.am not found");
	}

	while (fgets(content->line, sizeof(content->line), content->in) != NULL) {

		strcpy(content->lineCopy, content->line);
		if (strncmp(content->line, ";", 1) == 0) {/*if there is a comment line*/
			(content->lineNumber)++;
			continue;
		}
		remove_spaces(content->line);
		if (content->line[0] == '\0') {
			(content->lineNumber)++;
			continue;
		}

		if (strchr(content->line, ':')) {/*if there is a label*/

			strcpy(content->labelName, strtok(content->lineCopy, ":"));
			if (check_digit(content->labelName, 6, &content->lineNumber, &content->error) == 1){
				content->error = 1;
			}
			strcpy(content->macroName, "mcro");
			if (check_duplicate_macro(content, temp_macro)) {
				printf("Error: in line %d- The label named as macro name\n", content->lineNumber);
				content->error = 1;
			}
			temp = strtok(NULL, " \t\n");
			if (temp == NULL) {
				printf("Error: in line %d- There is only label without command.\n", content->lineNumber);
				content->error = 1;
				continue;
			}
			strcpy(content->command, temp);
			if (strcmp(content->command,".entry") != 0 && strcmp(content->command,".extern") != 0) {
				check_command(content);
			}
		}
		else {
			strcpy(content->command, strtok(content->lineCopy, " \t\n"));
		}

		temp = strtok(NULL, "\n");
		if (temp != NULL) {
			strcpy(content->totalLine, temp);
		}
		else {
			content->totalLine[0] = '\0';
		}

		if (strcmp(content->command, ".data") == 0 || strcmp(content->command, ".string") == 0 || strcmp(content->command, ".mat") == 0) {
			process_data_directive(content);
		}
		else if (strcmp(content->command, ".extern") == 0) {
			if (ex_en(content)==0) {
				check_command(content);
			}
		}
		else if (strcmp(content->command, ".entry") == 0) {
			if (ex_en(content) == 0) {
				if (add_ent_label(&content->symbol_ent,content->totalLine) == 1) {
					printf("Error: in line %d - Label \"%s\" already exists\n", content->lineNumber, content->totalLine);
					content->error = 1;
				}
			}
		}
		else {
			int words = count_words_for_instruction(content);
			content->IC += words;
		}
	(content->lineNumber)++;
	}

	fclose(content->in);
	fclose(temp_macro);
	remove("temp_macro.am");
	if (content->error > 0) {
	return 1;
	}
	return 0;
}

/*Check and write the command to label tabel*/
void check_command(assembler_context *content) {
	char temp[10];
	remove_spaces(content->command);
	remove_spaces(content->labelName);
	if (strcmp(content->command, ".data") == 0 || strcmp(content->command, ".string") == 0 || strcmp(content->command, ".mat") == 0) {
		if (add_label(&content->symbol_table_head, content->labelName, content->DC+content->IC, "data", "")){
			printf("Error: in line %d - Label \"%s\" already exists as internal/external. \n", content->lineNumber, content->labelName);
			content->error = 1;
		}
	}
	else if (strcmp(content->command, ".extern") == 0) {
		if (add_label(&content->symbol_table_head, content->totalLine, 0, "extern", "")){
			printf("Error: in line %d - Label \"%s\" already exists as internal/external.\n", content->lineNumber, content->labelName);
			content->error = 1;
		}
	}
	else if (opcode(content->command,temp) !=-1) {
		if (add_label(&content->symbol_table_head, content->labelName, content->IC+content->DC, "code", "")){
			printf("Error: in line %d - Label \"%s\" already exists as internal/external.\n", content->lineNumber, content->labelName);
			content->error = 1;
		}
	}
}

/*Handles data related commands in the assembly file.*/
void process_data_directive(assembler_context *content) {
	char *name = (char *)malloc(11 * sizeof(char));
	char operands_copy[82];
	char ascii[12];
	char *token;
	int i;
	name[0] = '\0';
	if (strcmp(content->command, ".data") == 0) {
		strcpy(operands_copy, content->totalLine);
		remove_spaces(operands_copy);
		if (check_digit(operands_copy, 5, &content->lineNumber, &content->error) == 1){
			content->error = 1;
		}
		token = strtok(operands_copy, ",\t\n");
		while (token != NULL) {
			dec_to_bin(token,name,10);
			Bin_line(&content->symbol_bin_code, name);
			(content->DC)++;
			token = strtok(NULL, ",\t\n");
		}
	}
	else if (strcmp(content->command, ".string") == 0) {
		if (content->totalLine[0] == '\"') {
			int length = 0;
			for (i=1; content->totalLine[i] != '\"' && content->totalLine[i] != '\0'; i++) {
				sprintf(ascii, "%d", (int)content->totalLine[i]);
				if (content->totalLine[i] < ' ' || content->totalLine[i] > '~'){
					printf("Error: in line %d - The character %c cannot be used.\n", content->lineNumber, content->totalLine[i]);
				content->error = 1;
				}
				dec_to_bin(ascii,name,10);
				Bin_line(&content->symbol_bin_code, name);
				length++;
			}
			(content->DC) += (length + 1);
			strcpy(name,"0000000000");
			Bin_line(&content->symbol_bin_code, name);
		}
	}
	else if (strcmp(content->command, ".mat") == 0) {
		int matNum = 1;
		int number = 0;
		strcpy(operands_copy, content->totalLine);
		token = strtok(operands_copy, " \t\n");
		number = check_digit(operands_copy, 4, &content->lineNumber, &content->error);
		if (number == 1){
			content->error = 1;
		}
		if (token != NULL) {token = strtok(NULL, "\n\r");}
		remove_spaces(token);
		if (check_digit(token, 5, &content->lineNumber, &content->error) == 1){
			content->error = 1;
		}
		token = strtok(token, ",");
		while (matNum <= number) {
			dec_to_bin(token,name,10);
			Bin_line(&content->symbol_bin_code, name);
			(content->DC)++;
			if (((token = strtok(NULL, ",\n\r\0")) == NULL) && matNum < number){
				token = "0";
			}
			matNum++;
		}
		if (number != 1 && token != NULL) {
			printf("Error: in line %d - There are extra numbers to put in this matrix.\n", content->lineNumber);
		content->error = 1;
		}
	}
	free(name);
}

/*Count the lines for instruction*/
int count_words_for_instruction(assembler_context *content) {
	char *name = (char *)malloc(11 * sizeof(char));
	int count = 1;
	int type = -1, mode1 = -1, mode2 = -1;
	char *operand1 = NULL, *operand2 = NULL, *operandExtra = NULL;
	char lineCopy[82];
	name[0] = '\0';
	strcpy(lineCopy, content->totalLine);
	type = opcode(content->command,name);
	if (type == -1) {
		printf("Error: in line %d- The command is undefined\n", content->lineNumber);
		content->error = 1;
	}

	if (strcmp(name,"1110") == 0 || strcmp(name,"1111") == 0) {
		strcat(name, "000000");
		Bin_line(&content->symbol_bin_code, name);
	}

	if (content->totalLine == NULL || strlen(content->totalLine) == 0) {
		if (type == 1 || type == 2) {
			printf("Error: in line %d- Missing operands\n", content->lineNumber);
			content->error = 1;
		}
		return count;
	}
	if (type == 1) {
		operand1 = NULL;
		operand2 = strtok(lineCopy, ", \t\n");
		operandExtra = strtok(NULL, ",\t\n");
	}
	else if (type == 2) {
		operand1 = strtok(lineCopy, ", \t\n");
		operand2 = strtok(NULL, ", \t\n");
		operandExtra = strtok(NULL, ",\t\n");
	}
	else if (type == 0) {
		operandExtra = strtok(lineCopy, ",\t\n");
	}

	if (operandExtra != NULL) {
		printf("Error: in line %d- There is extra operand\n", content->lineNumber);
		content->error = 1;
	}



	if (operand1 != NULL) {
		remove_spaces(operand1);
		mode1 = get_addressing_mode(operand1);
		if (check_Addressing_Method(content->command, 1, &mode1) == 1){
			printf("Error: in line %d- The operand 1 doesn't match the addressing method.\n", content->lineNumber);
		content->error = 1;
		}
		if (mode1 == 0){
			if(check_digit(operand1, 0, &content->lineNumber, &content->error) == 1){
				content->error = 1;
			}
			strcat(name, "00");
			count += 1;
		}
		else if (mode1 == 1){
			strcat(name, "01");
			count += 1;
		}
		else if (mode1 == 2) {
			if(check_digit(operand1, 2, &content->lineNumber, &content->error) == 1){
				content->error = 1;
			}
			strcat(name, "10");
			count += 2;
		}
		else if (mode1 == 3) {
			if(check_digit(operand1, 3, &content->lineNumber, &content->error) == 1){
				content->error = 1;
			}
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
		if (check_Addressing_Method(content->command, 2, &mode2) == 1){
			printf("Error: in line %d- The operand 2 doesn't match the addressing method.\n", content->lineNumber);
		content->error = 1;
		}
		if (mode2 == 0) {
			if(check_digit(operand2, 0, &content->lineNumber, &content->error) == 1){
				content->error = 1;
			}
			strcat(name, "00");
			count += 1;
		}
		else if (mode2 == 1) {
			strcat(name, "01");
			count += 1;
		}
		else if (mode2 == 2){
			if(check_digit(operand2, 2, &content->lineNumber, &content->error) == 1){
				content->error = 1;
			}
			strcat(name, "10");
			count += 2;
		}
		else if (mode2 == 3) {
			if(check_digit(operand2, 3, &content->lineNumber, &content->error) == 1){
				content->error = 1;
			}
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
	Bin_line(&content->symbol_bin_code, name);
	if (content->error == 0){ check_bin(&content->symbol_bin_code,operand1,operand2);}
	free(name);
	return count;
}

/*Check the type operand*/
int get_addressing_mode(char *operand) {
	if (operand[0] == '#') {
		return 0;
	}
	if (strchr(operand, '[') != NULL && strchr(operand, ']') != NULL) {
		return 2;
	}
	if (strchr(operand, 'r') != NULL) {
		return 3;
	}
	return 1;
}

/*Converts words to binary code*/
char check_bin(BinCode **symbol_bin_code, char *operand1, char *operand2) {
	char *temp = (char *)malloc(11 * sizeof(char));
	char *name = (char *)malloc(11 * sizeof(char));
	char *name1 = (char *)malloc(11 * sizeof(char));
	int mode1 = -1, mode2 = -1;
	char *r1,*r2;
	char operand_copy[82];
	temp[0] = '\0';
	name[0] = '\0';
	name1[0] = '\0';

	if (operand1 != NULL) {
		strcpy(operand_copy, operand1);
		mode1 = get_addressing_mode(operand1);
		if (mode1 == 0) {
			dec_to_bin(operand1+1, name, 8);
			strcat(name,"00");
			Bin_line(symbol_bin_code, name);
		}
		else if (mode1 == 1) {
			Bin_line(symbol_bin_code, operand1);
		}
		else if (mode1 == 2) {
			r1 = strchr(operand1, 'r');
			dec_to_bin(r1+1, temp, 4);
			strcat(name1, temp);
			r2 = strchr(r1+1, 'r');
			dec_to_bin(r2+1, temp, 4);
			strcat(name1, temp);
			strcat(name1,"00");
			Bin_line(symbol_bin_code,strtok(operand_copy,"["));
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
		strcpy(operand_copy, operand2);
		mode2 = get_addressing_mode(operand2);
		if (mode2 == 0) {
			dec_to_bin(operand2+1, name, 8);
			strcat(name,"00");
			Bin_line(symbol_bin_code, name);
		}
		else if (mode2 == 1) {
			Bin_line(symbol_bin_code, operand2);
		}
		else if (mode2 == 2) {
			r1 = strchr(operand2, 'r');
			dec_to_bin(r1+1, temp, 4);
			strcat(name1, temp);
			r2 = strchr(r1+1, 'r');
			dec_to_bin(r2+1, temp, 4);
			strcat(name1, temp);
			strcat(name1,"00");
			Bin_line(symbol_bin_code,strtok(operand_copy,"["));
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