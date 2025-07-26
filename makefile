all: assembler
assembler: assembler.o macros.o
	gcc -ansi -Wall -pedantic -o assembler assembler.o macros.o
assembler.o: assembler.c
	gcc -ansi -Wall -pedantic -c assembler.c
macros.o: macros.c
	gcc -ansi -Wall -pedantic -c macros.c
clean:
	rm *.o assembler