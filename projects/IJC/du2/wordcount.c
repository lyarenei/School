// wordcount.c
// Řešení IJC-DU2, příklad b), 30.3.2017
// Autor: Dominik Křivohlávek, FIT
// Přeloženo: gcc 6.3.1
// Program pro vypis vyskytu slov ze souboru / stdin

#include <stdio.h>
#include <stdlib.h>
#include "libhtab.h"

// Maximalni delka slova + '\0'
#define LINE_LEN 128

#ifndef HASH_FUNCTION
#define HASH_FUNCTION

// Moje hash_function
unsigned int hash_function(const char *str)
{
	unsigned i = 0;
    unsigned retval = 1;

    while(str[i] != '\0')
        retval *= (unsigned)str[i++];
    
    retval++;
	i++;
    return retval /= i;
}

#endif /* HASH_FUNCTION */

// Nacita slova ze stdin
// Vraci pocet nactenych znaku
// V pripade chyby vraci -1
int get_word(char *s, int max, FILE *f);

// Tiskne data ze zaznamu na stdout
void print(const char *key, unsigned *valueptr)
{
	if(key == NULL || valueptr == NULL)
	{
		fprintf(stderr, "Predany spatne ukazatele!\n");
		return;
	}
	printf("%s\t%u\n", key, *valueptr);
}

int main(void)
{
	// Buffer pro slovo
	char buffer[LINE_LEN];

	// Vytvoreni tabulky, (zduvodneni velikosti je nize)
	htab_t *table = htab_init(58889);

	if(table == NULL)
	{
		fprintf(stderr,"Nezdarilo se alokovat potrebne misto pro tabulku!\n");
		return EXIT_FAILURE;
	}

	// Nacitani slov
	while (get_word(buffer, LINE_LEN-1, stdin) != EOF)
	{
		if(strlen(buffer) == 0)
			continue; 
		if(htab_lookup_add(table, buffer) == NULL)
		{
			fprintf(stderr, "Chyba pri operaci v hash tabulce!\n");
			htab_free(table);
			return EXIT_FAILURE;
		}
	}

	// Vypis obsahu tabulky
	htab_foreach(table, &print);
	
	// Uvolneni tabulky z pameti
	htab_free(table);

	return EXIT_SUCCESS;
}

/*
Oduvodneni velikosti hash tabulky:

1) Cislo udavajici velikost hash tabulky by melo byt idealne prvocislo,
aby se minimalizovalo shlukovani zaznamu okolo urcitych indexu
a tedy i minimalizace pripadnych kolizi. 

2) Experimentalne jsem zjistil, ze pocet nevyuzitych ukazatelu na zaznamy 
v poli tabulky, nijak vyznamne nezvysuje cas pri vyhledavani. 
Velikost tabulky muzu tedy zvolit vetsi, nez by postacovalo pro dany vstup, 
za cenu nepatrne vyssi uziti pameti kvuli nevyuzitym ukazatelum na zaznamy.
Zaroven tim take muze byt podporena minimalizace kolizi v hash tabulce. 

3) Prumerny pocet slov v anglicke knize je zhruba okolo 70 000 [1].
Odecteme statisticky pet nejcastejsich [2] slov:
"the" - okolo 5% = 3 400 slov
"be" - okolo 3.2% = 2 220 slov
"and" - okolo 2.7% = 1 900 slov
"of" - okolo 2.6% = 1 800 slov
"a" - okolo 2.6% = 1 800 slov

70 000 - 3 400 - 2 220 - 1 900 - 1 800 - 1 800 = 58 880 slov

Nejblizsi prvocislo k tomuto cislu je: 58 889

Toto cislo je tedy dle meho nazoru dobra velikost hash tabulky.

Zdroje:
[1] http://www.huffingtonpost.com/2012/03/09/book-length_n_1334636.html
[2] http://www.wordfrequency.info/free.asp?s=y
*/
