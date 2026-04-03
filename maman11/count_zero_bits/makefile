all: play
play: count_zero_bits.o main.o
gcc -ansi -Wall -pedantic -o play count_zero_bits.o main.o
count_zero_bits.o: count_zero_bits.c
gcc -ansi -Wall -pedantic -c count_zero_bits.c
main.o: main.c
gcc -ansi -Wall -pedantic -c main.c
clean:
rm *.o play

