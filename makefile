index: index.o
	gcc -Wall -pedantic -ansi index.o -o index
index.o: index.c
	gcc -c -Wall -pedantic -ansi index.c -o index.o
