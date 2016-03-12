
CC = gcc
CFLAGS = -O2 -lrt

#default: sltest
default: sltest addtest
SortedList.o: SortedList.c
	$(CC) $(CFLAGS) -c $^ -o $@

sltest: sltest.c SortedList.o
	$(CC) $(CFLAGS) $^ -o $@

addtest: addtest.c
	gcc -pthread -o addtest addtest.c
#sltest: sltest.o
#	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean

clean:
	-rm SortedList.o
	-rm sltest
	-rm addtest

