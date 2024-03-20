# Makefile for 'common' module

OBJS = pagedir.o

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50
CC = gcc

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

pagedir.o: pagedir.h
	$(CC) $(CFLAGS) ../libcs50/file.c pagedir.c -c

.PHONY: clean
test:


clean:
	rm -f *~ *.o *.a
	rm -f core