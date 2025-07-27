typedef struct {
	char name[31];
	int address;
	char type[10];      /* "code" or "data"*/
	char attribute[10]; /* "entry", "external", or ""*/
} Label;


void add_label(Label symbol_table[], int *num_labels, char *name, int address, char *type, char *attribute);
void check_command(Label symbol_table[], int *num_labels, char *label_name, char *command, int DC, int IC);
int check_data(char *command);
int num_of_operands(char *command);
void process_data_directive(char *directive, char *operands, int *DC);
int count_words_for_instruction(char *command_name, char *total_line);
int get_addressing_mode(char *operand);