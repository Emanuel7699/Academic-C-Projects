char *file_extension(char *filename, char *ending);
int read_file(char *filename);
int write_macro_to_file(FILE *in, FILE *temp_macro);
int check_macro_in_file(char *line, FILE *temp_macro, FILE *out) ;
int check_macro(char *line, int i);
int check_duplicate_macro(char *macro, char *macro_name, FILE *temp_macro);
int check_instruction(char *macro);
void close_files(FILE *in, FILE *out, FILE *temp_macro, char *newfile);