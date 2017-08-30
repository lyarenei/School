// htab_remove.c
// Řešení IJC-DU2, příklad b), 30.3.2017
// Autor: Dominik Křivohlávek, FIT
// Přeloženo: gcc 6.3.1
// Modul s funkci, ktera vyhleda a zrusi zadany item podle key v hash tabulce

#include "libhtab.h"

#ifndef HTAB_REMOVE
#define HTAB_REMOVE

// Vraci false pokud neexistuje item
bool htab_remove(htab_t *t, const char *key)
{
    // Osetreni na nespravny ukazatel na tabulku
    if(t == NULL)
    {
        fprintf(stderr,"Byl predan neplatny ukazatel na tabulku!\n");
        return false;
    }

    // Vyhledani zaznamu; pokud existuje, uvolnime
    struct htab_listitem *item = htab_find(t, key);
    if(item == NULL)
        return false;
    else
    {
        // Vypocet indexu v tabulce pomoci hashovaci funkce
        unsigned index = (hash_function(key) % (t->arr_size));
        // Pomocny ukazatel na listitem predchazejici mazanemu listitemu
        struct htab_listitem *prev;

        // Nalezeni ukazatele na listitem predchazejici mazanemu listitemu
        for (prev = t->ptr[index]; prev->next != item; prev = prev->next);

        // Vyjmuti mazaneho listitemu ze seznamu a uvolneni mazaneho z pameti
        prev->next = item->next;
        free(item);
        t->n--;
        return true;
    }
}

#endif /* HTAB_REMOVE */
