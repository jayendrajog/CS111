CC=gcc
USER=JayendraJog
DIST_SOURCES = simpsh.c test.sh README

default: simpsh

simpsh.o: simpsh.c
	$(CC) -c simpsh.c -o simpsh.o

simpsh: simpsh.o
	$(CC) simpsh.o -o simpsh

clean:
	-rm -f simpsh.o
	-rm -f simpsh
	-rm -f lab1-$(USER).tar.gz

dist: lab1-$(USER).tar.gz

lab1-$(USER).tar.gz: $(DIST_SOURCES)
	tar -cvf lab1-$(USER).tar.gz $(DIST_SOURCES)
