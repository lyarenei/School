# Makefile
# Řešení IJC-DU2, 30.3.2017
# Autor: Dominik Křivohlávek, FIT
# Makefile pro druhou domácí úlohu předmětu IJC

# Preklad pro C
CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -g -fPIC -pedantic

# Preklad pro C++
CXX=g++
CXXFLAGS=-std=c++11 -Wall -Wextra -g -pedantic

# Parametry pro linker
LDFLAGS=-shared

# Moduly knihovny a jejich .o verze
SRCS= hash_function.c htab_bucket_count.c htab_clear.c htab_find.c htab_foreach.c htab_free.c htab_init.c htab_lookup_add.c htab_move.c htab_size.c
OBJS = $(SRCS:.c=.o)

# Preklad vsech programu z IJC-DU2
all: tail tail2 wordcount wordcount-dynamic

# Preklad tail-u napsanem v C
tail: tail.o
	$(CC) $(CFLAGS) tail.o -o tail

# Preklad tail-u napsanem v C++
tail2: tail2.o
	$(CXX) $(CXXFLAGS) tail2.o -o tail2

# Preklad wordcount (se statickou knihovnou)
wordcount: wordcount.o io.o libhtab.a
	$(CC) $(CFLAGS) wordcount.o io.o -L . libhtab.a -o wordcount

# Preklad wordcount (s dynamickou knihovnou)
wordcount-dynamic: wordcount.o io.o libhtab.so
	$(CC) $(CFLAGS) wordcount.o io.o -L . libhtab.so -o wordcount-dynamic

# Vytvoreni libhtab.a
libhtab.a: $(OBJS)
	ar rs libhtab.a $(OBJS)

# Vytvoreni libhtab.so
libhtab.so: $(OBJS)
	ld $(LDFLAGS) $(OBJS) -o libhtab.so
	
$(SRCS:.c=.d):%.d:%.c
	$(CC) $(CFLAGS) -MM $< >$@

# Vymazani souboru po prekladu
clean:
	rm *.o tail tail2 wordcount wordcount-dynamic libhtab.a libhtab.so
