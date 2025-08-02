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
void add_label(Label **head, char *name, int address, char *type, char *attribute);
void Bin_line(BinCode **head, char *name);
void check_command(Label **head, char *label_name, char *command, int DC, int IC);
void process_data_directive(BinCode **symbol_bin_code, char *directive, char *operands, int *DC);
int count_words_for_instruction(BinCode **symbol_bin_code,char *command_name, char *total_line);
int get_addressing_mode(char *operand);
void remove_spaces(char *str);
void print_label_list(Label *symbol_table_head, FILE *out);
void free_label_list(Label *symbol_table_head);
void print_bin_list(BinCode *head, FILE *out);
void free_bin_list(BinCode *head);
char opcode (char *command, char *name);
char check_bin(BinCode **symbol_bin_code, char *operand1, char *operand2);
void dec_to_bin(char *decimal_str, char *binary_str, int bits);