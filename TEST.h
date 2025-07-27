typedef struct {
	char name[31];
	int address;
	char type[10];      /* "code" or "data"*/
	char attribute[10]; /* "entry", "external", or ""*/
} Label;


int first_pass(char *filename);
void add_label(Label labels[], int *num_labels, char *name, int address, char *type, char *attribute);
int get_addressing_mode(char *operand);
int count_words_for_instruction(char *command_name, char *operand1, char *operand2);
void process_data_directive(char *directive, char *operands, int *DC);
int opcode (char *command);
