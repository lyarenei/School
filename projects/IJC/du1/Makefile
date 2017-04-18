# Makefile
# Řešení IJC-DU1, 24.3.2017
# Autor: Dominik Křivohlávek, FIT
# Makefile pro projekt IJC-DU1

CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -g

# Překlad všech programů z IJC-DU1
all: primes primes-i steg-decode

# Překlad programu primes s makry
primes: primes.o error.o eratosthenes.o
	$(CC) $(CFLAGS) primes.o error.o eratosthenes.o -o primes -lm

# Překlad programu primes s inline funkcemi
primes-i: primes.o error.o eratosthenes.o
	$(CC) $(CFLAGS) -DUSE_INLINE primes.o error.o eratosthenes.o -o primes-i -lm

# Překlad programu pro dekódování zprávy z obrázku .ppm
steg-decode: steg-decode.o error.o ppm.o eratosthenes.o
	$(CC) $(CFLAGS) steg-decode.o error.o ppm.o eratosthenes.o -o steg-decode -lm

# Spuštění programů primes a primes-i
run:
	time ./primes
	time ./primes-i

# Mazání všech objektových souborů včetně spustitelných
clean:
	rm *.o primes primes-i steg-decode