// htab_size.c
// Řešení IJC-DU2, příklad b), 30.3.2017
// Autor: Dominik Křivohlávek, FIT
// Přeloženo: gcc 6.3.1
// Modul s funkci pro zjisteni poctu zaznamu v hash tabulce

#include "libhtab.h"

#ifndef HTAB_SIZE
#define HTAB_SIZE

size_t htab_size(htab_t * t)
{
    // Osetreni na nespravny ukazatel na tabulku
    if(t == NULL)
        return -1;
    
    return t->n;
}

#endif /* HTAB_SIZE */
