// htab_clear.c
// Řešení IJC-DU2, příklad b), 30.3.2017
// Autor: Dominik Křivohlávek, FIT
// Přeloženo: gcc 6.3.1
// Modul s funkci, ktera dealokuje veskere buckety v hash tabulce

#include "libhtab.h"

#ifndef HTAB_CLEAR
#define HTAB_CLEAR

// Tabulku ponecha prazdnou
void htab_clear(htab_t *t)
{
    // Osetreni na nespravny ukazatel na tabulku
    if(t == NULL)
        return;
    
    struct htab_listitem *tmp;
    for (unsigned i = 0; i < htab_bucket_count(t); i++)
    {
        for(struct htab_listitem *pointer = t->ptr[i]; pointer != NULL; pointer = tmp)
        {
            tmp = pointer->next;
            free(pointer);
        }
        t->ptr[i] = NULL;
    }
    t->n = 0;
}

#endif /* HTAB_CLEAR */
