// htab_move.c
// Řešení IJC-DU2, příklad b), 30.3.2017
// Autor: Dominik Křivohlávek, FIT
// Přeloženo: gcc 6.3.1
// Modul s funkci, ktera vytvori novou hash tabulku spolu s inicializaci daty ze stavajici

#include "libhtab.h"

#ifndef HTAB_MOVE
#define HTAB_MOVE

// Tabulka t2 zustane prazdna a alokovana
// Pri chybe vraci NULL
htab_t * htab_move(int newsize, htab_t *t2)
{
    // Osetreni na nespravny ukazatel
	if(t2 == NULL)
	{
		fprintf(stderr, "Byl predan nespravny ukazatel na tabulku!\n");
		return NULL;
	}

    // Osetreni na velikost nove tabulky
	if(newsize < t2->arr_size)
	{
		fprintf(stderr, "Zadana velikost je mensi nez aktualni tabulka!\n");
		return NULL;
	}

    // Vytvoreni nove tabulky
	htab_t *newtab;
	newtab = htab_init(newsize);
	if(newtab == NULL)
	{
		fprintf(stderr, "Nezdarilo se alokovat misto pro novou tabulku!\n");
	}

    // Zapis informace o velikosti pole ukazatelu na zaznamy
	newtab->arr_size = newsize;
	
    // Kopie bucketu (je znovu pocitan index bucketu pres htab_lookup_add)
	struct htab_listitem *tmp;
	for (unsigned i = 0; i < htab_bucket_count(t2); i++)
	{
		for (struct htab_listitem *pointer = t2->ptr[i]; pointer != NULL; pointer = pointer->next)
		{
			tmp = htab_lookup_add(newtab, pointer->key);
			if (tmp == NULL)
			{
				fprintf(stderr, "Nezdarilo se alokovat pamet pro zaznam!\n");
				return NULL;
			}
			tmp->data = pointer->data;
		}
	}

	// Vyprazdneni puvodni tabulky
	htab_clear(t2);
	
	return newtab;
}

#endif /* HTAB_MOVE */
