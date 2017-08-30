// htab_foreach.c
// Řešení IJC-DU2, příklad b), 30.3.2017
// Autor: Dominik Křivohlávek, FIT
// Přeloženo: gcc 6.3.1
// Modul s funkci, ktera pro kazdy item v hash tabulce vola funkci 'function'

#include "libhtab.h"

#ifndef HTAB_FOREACH
#define HTAB_FOREACH

// V tabulce nic nemeni, vhodne napr pro vypis obsahu
void htab_foreach(htab_t *t, void (*function)(const char *key, unsigned *valueptr))
{
    // Osetreni na nespravny ukazatel na tabulku
    if(t == NULL)
    {
        fprintf(stderr,"Byl predan neplatny ukazatel na tabulku!\n");
        return;
    }
	// Iterace pres vsechny itemy v hash tabulce
    for (unsigned i = 0; i < htab_bucket_count(t); i++)
    {
        for (struct htab_listitem *pointer = t->ptr[i]; pointer != NULL; pointer = pointer->next)
        {
            function(pointer->key, &pointer->data);
        }  
    }
}

#endif /* HTAB_FOREACH */
