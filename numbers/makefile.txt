all: play
play: numbers.o
	gcc -ansi -Wall -pedantic -o play numbers.o
numbers.o: numbers.c
	gcc -ansi -Wall -pedantic -c numbers.c
clean:
	rm *.o play