# Makefile for IOS-Proj2
# Author: Dominik Křivohlávek
# Login: xkrivo02

# Compiler
CC=gcc

# Compiler flags
CFLAGS=-std=gnu99 -Wall -pedantic -Wextra -pthread -Werror

# Project compiling
proj2: proj2.o shvar_op.o sem_op.o
	$(CC) $(CFLAGS) proj2.o shvar_op.o sem_op.o -o proj2

# Clean all files
clean:
	rm proj2 *.o proj2.out