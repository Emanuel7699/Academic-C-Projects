all: assembler
assembler: assembler.o
	gcc -ansi -Wall -pedantic -o assembler assembler.o
assembler.o: assembler.c
	gcc -ansi -Wall -pedantic -c assembler.c
clean:
	rm *.o assembler