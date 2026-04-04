all: play
play: palindrome.o main.o
gcc -ansi -Wall -pedantic -o play palindrome_1.o main.o
palindrome.o: palindrome.c
gcc -ansi -Wall -pedantic -c palindrome.c
main.o: main.c
gcc -ansi -Wall -pedantic -c main.c
clean:
rm *.o play

