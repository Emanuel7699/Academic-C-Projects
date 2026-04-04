char *file_extension(char *filename, char *ending);
int read_file(assembler_context *content);
int write_macro_to_file(assembler_context *content, FILE *temp_macro);
int check_macro_in_file(assembler_context *content, FILE *temp_macro);
int check_macro(int i, assembler_context *content);
int check_duplicate_macro(assembler_context *content, FILE *temp_macro);
int check_instruction(char *macro);
void close_files(FILE *in, FILE *out, FILE *temp_macro);