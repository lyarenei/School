// htab_lookup_add.c
// Řešení IJC-DU2, příklad b), 30.3.2017
// Autor: Dominik Křivohlávek, FIT
// Přeloženo: gcc 6.3.1
// Modul s funkci pro vyhledani / vytvoreni itemu podle klice v hash tabulce

#include "libhtab.h"

#ifndef HTAB_LOOKUP_ADD
#define HTAB_LOOKUP_ADD

// Pokud key neexistuje, vytvori item a vraci ukazatel na nej ukazatel
// Pri chybe vraci NULL
struct htab_listitem * htab_lookup_add(htab_t *t, const char *key)
{
    // Osetreni na nespravny ukazatel na tabulku
    if(t == NULL)
    {
        fprintf(stderr,"Byl predan neplatny ukazatel na tabulku!\n");
        return NULL;
    }

    // Vypocet indexu v tabulce pomoci hashovaci funkce
    unsigned index = (hash_function(key) % (t->arr_size));

    // Pokud neni na zadanem indexu zadny seznam, vytvorime jej
    if(t->ptr[index] == NULL)
    {
        t->ptr[index] = malloc(sizeof(struct htab_listitem));
        if(t->ptr[index] == NULL)
        {
            fprintf(stderr,"Nezdarilo se alokovat misto pro htab_listitem!\n");
            return NULL;
        }
        // Inicializace noveho htab_listitem
        t->ptr[index]->data = 1;
        strcpy(t->ptr[index]->key, key);
        t->ptr[index]->next = NULL;
        t->n++;
        return t->ptr[index];
    }

    // Pokud zaznam jiz existuje, projdeme jej a testujeme shodu klicu
    for(struct htab_listitem *pointer = t->ptr[index]; pointer != NULL; pointer = pointer->next)
    {
        if(!strcmp(pointer->key, key))
        {
            pointer->data++;
            return pointer;
        }
    }

    // Key neexistuje; vytvoreni noveho itemu
    struct htab_listitem *tmp = t->ptr[index]; // Pomocna promenna pro zapis 'next' itemu
    t->ptr[index] = malloc(sizeof(struct htab_listitem));
    if(t->ptr[index] == NULL)
    {
        fprintf(stderr,"Nezdarilo se alokovat misto pro htab_listitem!\n");
        return NULL;
    }
    // Inicializace noveho htab_listitem
    t->ptr[index]->data = 1;
    strcpy(t->ptr[index]->key, key);
    t->ptr[index]->next = tmp;
    t->n++;
    return t->ptr[index];
}

#endif /* HTAB_LOOKUP_ADD */
