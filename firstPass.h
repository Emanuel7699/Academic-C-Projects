typedef struct Label{
	char name[31];
	int address;
	char type[10];      /* "code" or "data"*/
	char attribute[10]; /* "entry", "external", or ""*/
	struct Label *next;
} Label;

int first_pass(char *filename);
void add_label(Label **head, char *name, int address, char *type, char *attribute);
void check_command(Label **head, char *label_name, char *command, int DC, int IC);
int check_data(char *command);
int num_of_operands(char *command);
void process_data_directive(char *directive, char *operands, int *DC);
int count_words_for_instruction(char *command_name, char *total_line);
int get_addressing_mode(char *operand);
void remove_spaces(char *str);
void print_label_list(Label *symbol_table_head, FILE *out);
void free_label_list(Label *symbol_table_head);