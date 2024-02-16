# Makefile for indexer

OBJS = querier.o ../common/index.o ../common/pagedir.o
LLIBS = ../libcs50/libcs50.a ../common/common.a
LIBS = 

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50  -I../common
CC = gcc

all: querier

querier: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(LIBS) -o querier

.PHONY: test clean


test: querier
	./testing.sh

clean:
	rm -f *~ *.o
	rm -f core
	rm -f ./querier