// htab_init.c
// Řešení IJC-DU2, příklad b), 30.3.2017
// Autor: Dominik Křivohlávek, FIT
// Přeloženo: gcc 6.3.1
// Modul s funkci pro alokaci a inicializaci hash tabulky
// Fukce pri chybe vraci NULL

#include "libhtab.h"

#ifndef HTAB_INIT
#define HTAB_INIT

htab_t * htab_init(int size)
{
    // Osetreni na nespravnou velikost
    if(size <= 0)
    {
        fprintf(stderr,"Byla predana nespravna velikost tabulky\n");
        return NULL;
    }
    // Alokovani hash tabulky (2x int + pozadovana velikost ukazatelu na list zaznamu)
    struct htab_t *table = malloc((2*sizeof(int))+(size*sizeof(struct htab_listitem*)));
    
    // Osetreni proti selhani alokace
    if(table == NULL)
    {
        fprintf(stderr,"Nezdarilo se alokovat misto pro tabulku\n");
        return NULL;
    }
    
    // Inicializace hodnot v tabulce
    table->arr_size = size;
    table->n = 0; 
    for (int i = 0; i < size; i++)
        table->ptr[i] = NULL;
    
    return table;
}

#endif /* HTAB_INIT */
