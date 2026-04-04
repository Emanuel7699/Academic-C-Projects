all: assembler
assembler: assembler.o macros.o firstPass.o util.o secondPass.o
	gcc -ansi -Wall -pedantic -o assembler assembler.o macros.o firstPass.o util.o secondPass.o
assembler.o: assembler.c
	gcc -ansi -Wall -pedantic -c assembler.c
macros.o: macros.c
	gcc -ansi -Wall -pedantic -c macros.c
firstPass.o: firstPass.c
	gcc -ansi -Wall -pedantic -c firstPass.c
util.o: util.c
	gcc -ansi -Wall -pedantic -c util.c
secondPass.o: secondPass.c
	gcc -ansi -Wall -pedantic -c secondPass.c
clean:
	rm *.o assembler