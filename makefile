all: play
play: mymat.o mainmat.o
	gcc -ansi -Wall -pedantic -o play mymat.o mainmat.o
mymat.o: mymat.c
	gcc -ansi -Wall -pedantic -c mymat.c
mainmat.o: mainmat.c
	gcc -ansi -Wall -pedantic -c mainmat.c
clean:
	rm *.o play