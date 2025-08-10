typedef struct Label{
	char name[31];
	int address;
	char type[10];      /* "code" or "data"*/
	char attribute[10]; /* "entry", "external", or ""*/
	struct Label *next;
} Label;

typedef struct BinCode{
	char name[11];
	struct BinCode *next;
} BinCode;

int first_pass(char *filename);
void check_command(Label **head, char *label_name, char *command, int DC, int IC, int *error, int lineNumber);
void process_data_directive(BinCode **symbol_bin_code, char *directive, char *operands, int *DC, int *error, int lineNumber);
int count_words_for_instruction(BinCode **symbol_bin_code,char *command_name, char *total_line, int lineNumber, int *error);
int get_addressing_mode(char *operand);
char check_bin(BinCode **symbol_bin_code, char *operand1, char *operand2);