
PRJ=c206
#
PROGS=$(PRJ)-test
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm -g

all: $(PROGS)

$(PRJ)-test: $(PRJ).c $(PRJ)-test.c
	$(CC) $(CFLAGS) -o $@ $(PRJ).c $(PRJ)-test.c

clean:
	rm -f *.o *.out $(PROGS)
#