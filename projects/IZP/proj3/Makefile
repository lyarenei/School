CC=gcc
CFLAGS=--std=c99 -Wall -Wextra -Werror -DNDEBUG -g -lm

proj1: proj3.c
	$(CC) $(CFLAGS) proj3.c -o proj3

clean:
	rm proj3
gdb:
	gdb -tui ./proj3
