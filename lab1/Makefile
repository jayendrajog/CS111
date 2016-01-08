CC=gcc

default: simpsh

simpsh.o: simpsh.c
	$(CC) -c simpsh.c -o simpsh.o

simpsh: simpsh.o
	$(CC) simpsh.o -o simpsh

clean:
	-rm -f simpsh.o
	-rm -f simpsh
