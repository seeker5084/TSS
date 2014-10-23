#
#   [Makefile] - a make file
#
#    This program encode ASCII text
#   with Huffman-coding.
#
#   > Date: April 7 2014
#   > Contact: seeker5084@gmail.com
#   > Written by:
#

all: huffman

huffman.o: huffman.c huffman.h
	gcc -c huffman.c

main.o: main.c huffman.h
	gcc -c main.c

huffman: huffman.o main.o
	gcc huffman.o main.o -o huffman

clean:
	rm -rf *.o huffman
