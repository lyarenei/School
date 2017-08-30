// htab_bucket_count.c
// Řešení IJC-DU2, příklad b), 30.3.2017
// Autor: Dominik Křivohlávek, FIT
// Přeloženo: gcc 6.3.1
// Modul s funkci pro zjisteni maximalniho poctu bucketu v hash tabulce

#include "libhtab.h"

#ifndef HTAB_BUCKET_COUNT
#define HTAB_BUCKET_COUNT

size_t htab_bucket_count(htab_t * t)
{
    // Osetreni na nespravny ukazatel na tabulku
    if(t == NULL)
        return -1;
    
    return t->arr_size;
}

#endif /* HTAB_BUCKET_COUNT */
