int first_pass(assembler_context *content);
void check_command(assembler_context *content);
void process_data_directive(assembler_context *content);
int count_words_for_instruction(assembler_context *content);
int get_addressing_mode(char *operand);
char check_bin(BinCode **symbol_bin_code, char *operand1, char *operand2);