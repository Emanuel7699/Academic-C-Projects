all: play
play: sum.o magic.o matrix.o
	gcc -ansi -Wall -pedantic -o play sum.o main.o matrix.o
matrix.o: matrix.c
	gcc -ansi -Wall -pedantic -c matrix.c
sum.o: sum.c
	gcc -ansi -Wall -pedantic -c sum.c
magic.o: magic.c
	gcc -ansi -Wall -pedantic -c magic.c
clean:
	rm *.o play