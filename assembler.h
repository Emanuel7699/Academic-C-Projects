typedef struct assembler_context {
	FILE *in, *out;
	Label *symbol_table_head;
	BinCode *symbol_bin_code;
	Ent *symbol_ent;

	char macro[10], macro_name[82], line[82], lineCopy[82], totalLine[82];
	char labelName[32];
	char command[31];
	char *temp;
	char *newfile;
	int IC, DC;
	int error;
	int lineNumber;
} assembler_context;


char *file_extension(char *filename, char *ending);
int read_file(char *filename);
int write_macro_to_file(FILE *in, FILE *temp_macro, int *lineNumber);
int check_macro_in_file(char *line, FILE *temp_macro, FILE *out) ;
int check_macro(char *line, int i, int *lineNumber);
int check_duplicate_macro(char *macro, char *macro_name, FILE *temp_macro);
int check_instruction(char *macro);
void close_files(FILE *in, FILE *out, FILE *temp_macro);