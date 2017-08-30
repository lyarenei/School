// htab_free.c
// Řešení IJC-DU2, příklad b), 30.3.2017
// Autor: Dominik Křivohlávek, FIT
// Přeloženo: gcc 6.3.1
// Modul s funkci, ktera dealokuje celou hash tabulku

#include "libhtab.h"

#ifndef HTAB_FREE
#define HTAB_FREE

void htab_free(htab_t *t)
{
    // Osetreni na nespravny ukazatel na tabulku
    if(t == NULL)
        return;
    
    htab_clear(t);
    free(t);
    t = NULL;
}

#endif /* HTAB_FREE */
