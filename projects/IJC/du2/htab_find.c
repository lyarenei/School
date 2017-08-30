// htab_find.c
// Řešení IJC-DU2, příklad b), 30.3.2017
// Autor: Dominik Křivohlávek, FIT
// Přeloženo: gcc 6.3.1
// Modul s funkci pro vyhledani itemu podle klice v hash tabulce

#include "libhtab.h"

#ifndef HTAB_LISTITEM
#define HTAB_LISTITEM

// Pokud item s klicem neexistuje, vraci NULL
struct htab_listitem * htab_find(htab_t *t, const char *key)
{
    // Osetreni na nespravny ukazatel na tabulku
    if(t == NULL)
    {
        fprintf(stderr,"Byl predan neplatny ukazatel na tabulku!\n");
        return NULL;
    }

    // Vypocet indexu v tabulce pomoci hashovaci funkce
    unsigned index = (hash_function(key) % (t->arr_size));

    // Pokud neni na zadanem indexu zadny seznam; vratime rovnou NULL
    if(t->ptr[index] == NULL)
        return t->ptr[index];

    // Pokud zaznam jiz existuje, projdeme jej a testujeme shodu klicu
    for (struct htab_listitem *pointer = t->ptr[index]; pointer != NULL; pointer = pointer->next)
    {
        if(!strcmp(pointer->key, key))
            return pointer;
    }

    // Key neexistuje; vracime NULL
    return NULL;
}

#endif /* HTAB_LISTITEM */
