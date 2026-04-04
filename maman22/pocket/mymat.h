#include <stdio.h>

typedef float mat[4][4];
typedef struct {
    char *command;
    mat **matrixs;
    float *values;
} ParsedCommand;

void read_mat(ParsedCommand *parsed);
void add_mat(ParsedCommand *parsed);
void sub_mat(ParsedCommand *parsed);
void mul_mat(ParsedCommand *parsed);
void mul_scalar(ParsedCommand *parsed);
void trans_mat(ParsedCommand *parsed);
void print_mat(mat *a);
char* trim(char *str);
int errors(ParsedCommand *error,char *input);