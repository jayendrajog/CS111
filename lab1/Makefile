CC=gcc
USER=JayendraJog
DIST_SOURCES = simpsh.c test.sh README Makefile report.pdf

default: simpsh

simpsh.o: simpsh.c
	$(CC) -c simpsh.c -o simpsh.o

simpsh: simpsh.o
	$(CC) simpsh.o -o simpsh

.PHONY: clean
clean:
	-rm -f simpsh.o
	-rm -f simpsh
	-rm -f lab1-$(USER).tar.gz

check:
	./test.sh

dist: lab1-$(USER).tar.gz

lab1-$(USER).tar.gz: $(DIST_SOURCES)
	rm -rf lab1-$(USER)
	mkdir lab1-$(USER)
	cp $(DIST_SOURCES) lab1-$(USER)
	tar -cvzf lab1-$(USER).tar.gz lab1-$(USER)
