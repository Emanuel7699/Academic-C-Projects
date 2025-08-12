int second_pass(char *filename, Label **symbol_table_head, BinCode **symbol_bin_code);
char check_line(BinCode **symbol_bin_code, Label **symbol_table_head, char *total_line);
void check_label(BinCode **symbol_bin_code, Label **symbol_table_head, char *name);
int check_guideline(char *line);