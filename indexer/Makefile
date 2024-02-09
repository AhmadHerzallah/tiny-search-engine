# Makefile for indexer

OBJS = indexer.o ../common/index.o
TOBJS = indextest.o ../common/index.o
LLIBS = ../libcs50/libcs50.a ../common/common.a
LIBS = 

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50  -I../common
CC = gcc

all: indexer indextest

indexer: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(LIBS) -o indexer

indextest: $(TOBJS)
	$(CC) $(CFLAGS) $(TOBJS) $(LLIBS) $(LIBS) -o indextest

.PHONY: test clean

test: indexer
	./testing.sh

clean:
	rm -f *~ *.o
	rm -f core
	rm -f ./indexer ./indextest
	rm -f ../data/indextesting/indextestresults.index
	rm -f ../data/letters/.index