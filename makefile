all: assembler
assembler: assembler.o macros.o
	gcc -ansi -Wall -pedantic -o assembler assembler.o macros.o
assembler.o: assembler.c
	gcc -ansi -Wall -pedantic -c assembler.c
macros.o: macros.c
	gcc -ansi -Wall -pedantic -c macros.c
firstPass.o: TEST.c
	gcc -ansi -Wall -pedantic -c firstPass.c
clean:
	rm *.o assembler