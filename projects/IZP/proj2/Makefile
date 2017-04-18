CC=gcc
CFLAGS=-std=c99 -Wall -Werror -Wextra -pedantic -g -lm

proj1: proj2.c
	$(CC) $(CFLAGS) proj2.c -o proj2

clean:
	rm proj2
gdb:
	gdb -tui ./proj2
