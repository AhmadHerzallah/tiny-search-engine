# # Makefile for 'crawler' module

# OBJS = crawler.o
# LLIBS = ../libcs50/libcs50.a ../common/pagedir.o ../common/index.o ../common/word.o
# LIBS = 

# CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50
# CC = gcc

# crawler: $(OBJS)
# 	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(LIBS) -o crawler

# .PHONY: test clean

# test: crawler
# 	./testing.sh

# clean:
# 	rm -f *~ *.o
# 	rm -f ./crawler
# 	rm -rf ../data/letters/* ../data/toscrape/* ../data/wikipedia/*
# 	rm -f ../data/letters/.crawler ../data/toscrape/.crawler ../data/wikipedia/.crawler

# Makefile for crawler

OBJS = crawler.o
LLIBS = ../libcs50/libcs50.a ../common/common.a
LIBS = 

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50
CC = gcc

crawler: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(LIBS) -o crawler

.PHONY: test clean

test: crawler
	./testing.sh

clean:
	rm -f *~ *.o
	rm -f ./crawler
	rm -rf ../data/letters/* ../data/toscrape/* ../data/wikipedia/*
	rm -f ../data/letters/.crawler ../data/toscrape/.crawler ../data/wikipedia/.crawler