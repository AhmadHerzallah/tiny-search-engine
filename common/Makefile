# Makefile for common

OBJS = pagedir.o word.o index.o
LIB = common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50
CC = gcc

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

index.o: index.h
	$(CC) $(CFLAGS) ../libcs50/libcs50.a index.c -c

word.o: word.h
	$(CC) $(CFLAGS) word.c -c

pagedir.o: pagedir.h
	$(CC) $(CFLAGS) ../libcs50/file.c pagedir.c -c

.PHONY: clean

clean:
	rm -f *~ *.o *.a
	rm -f core