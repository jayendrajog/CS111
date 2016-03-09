CC = gcc
CFLAGS = -O2 -lrt

#default: sltest
default: sltest.o
SortedList.o: SortedList.c
	$(CC) $(CFLAGS) -c $^ -o $@

sltest.o: sltest.c SortedList.o
	$(CC) $(CFLAGS) $^ -o $@

#sltest: sltest.o
#	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean

clean:
	-rm SortedList.o
	-rm sltest.o
