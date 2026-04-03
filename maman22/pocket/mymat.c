#include <stdio.h>
#include "mymat.h"


void read_mat(ParsedCommand *parsed){
	int i=0,j=0;
	for (i=0; i<4; i++){
		for (j=0; j<4; j++){
			(*(*(parsed->matrixs)))[i][j] = parsed->values[i * 4 + j];
		}
	}
}

void add_mat(ParsedCommand *parsed){
	int i=0,j=0;
	for (i=0; i<4; i++){
		for (j=0; j<4; j++){
		    (*(*(parsed->matrixs + 2)))[i][j] = (*(*(parsed->matrixs)))[i][j] + (*(*(parsed->matrixs + 1)))[i][j];
		}
	}
}

void sub_mat(ParsedCommand *parsed){
	int i=0,j=0;
	for (i=0; i<4; i++){
		for (j=0; j<4; j++){
		(*(*(parsed->matrixs + 2)))[i][j] = (*(*(parsed->matrixs)))[i][j] - (*(*(parsed->matrixs + 1)))[i][j];
		}
	}
}

void mul_mat(ParsedCommand *parsed){
	int i=0,j=0,k=0;
	mat temp = {0};
	for (i=0; i<4; i++){
		for (j=0; j<4; j++){
            for (k=0; k<4; k++) {
                temp[i][j] += (*(*(parsed->matrixs)))[i][k] * (*(*(parsed->matrixs + 1)))[k][j];
            }
		}
	}
	for (i=0; i<4; i++){
		for (j=0; j<4; j++){
			(*(*(parsed->matrixs + 2)))[i][j] = temp[i][j];
		}
	}
}

void mul_scalar(ParsedCommand *parsed){
	int i=0,j=0;
	for (i=0; i<4; i++){
		for (j=0; j<4; j++){
			(*(*(parsed->matrixs + 1)))[i][j] = *(parsed->values) * (*(*(parsed->matrixs)))[i][j];
		}
	}
}

void trans_mat(ParsedCommand *parsed){
    int i=0,j=0;
    if ((*(parsed->matrixs+1))==(*(parsed->matrixs))){
		int temp = 0;
		for (i=0; i<4; i++){
			for (j=1+i; j<4; j++){
				temp = (*(*(parsed->matrixs)))[i][j];
				(*(*(parsed->matrixs)))[i][j] = (*(*(parsed->matrixs)))[j][i];
				(*(*(parsed->matrixs)))[j][i] = temp;
			}
		}
	}
	else{
		for (i=0; i<4; i++){
			for (j=0; j<4; j++){
				(*(*(parsed->matrixs + 1)))[i][j] = (*(*(parsed->matrixs)))[j][i];
			}
		}
	}
}

void print_mat(mat *a){
	int i=0,j=0;
	printf("Matrix:\n");
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			printf("%7.2f ",(*a)[i][j]);
		}
		printf("\n");
	}
}

