// libhtab.h
// Řešení IJC-DU2, příklad b), 30.3.2017
// Autor: Dominik Křivohlávek, FIT
// Přeloženo: gcc 6.3.1
// Hlavičkový soubor pro libtab.c

#ifndef LIBHTAB_H
#define LIBHTAB_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Hash tabulka
typedef struct htab_t
{
    int arr_size;                   // Velikost pole ukazatelu na seznamy zaznamu
    int n;                          // Aktualni pocet zaznamu
    struct htab_listitem *ptr[];    // Poke ukazatelu na seznam zaznamu
} htab_t;

// Zaznam v tabulce (key)
struct htab_listitem
{
    unsigned data;                          // Pocet vyskytu
    struct htab_listitem *next;        // Ukazatel na dalsi zaznam
    char key[127];                     // Retezec v tabulce
};

// Rozptylovaci funkce (pro vypocet indexu v tabulce)
unsigned int hash_function(const char *str);

// Vytvoreni a inicializace tabulky,
// pri chybe vraci NULL
htab_t * htab_init(int size);

// Vytvoreni a inicializace tabulky daty z tabulky t2,
// t2 poté zůstane prázdná a alokovaná,
// Pri chybe vraci NULL
htab_t * htab_move(int newsize, htab_t *t2);

// Vraci pocet prvku tabulky
size_t htab_size(struct htab_t * t);

// Vraci pocet prvku pole (arr_size)
size_t htab_bucket_count(struct htab_t * t);

// Vyhleda odpovidajici zaznam dle key
// Pokud neexistuje, vytvori jej a vraci ukazatel
// Pri chybe vraci NULL
struct htab_listitem * htab_lookup_add(htab_t *t, const char *key);

// Vyhleda odpovidajici zaznam dle key
// Pokud neexistuje, vraci NULL
struct htab_listitem * htab_find(htab_t *t, const char *key);

// Vola funkci func pro kazdy prvek
// V tabulce nic nemeni, vhodne napr pro vypis obsahu
void htab_foreach(htab_t *t, void (*function)(const char *key, unsigned *valueptr));

// Vyhleda a zrusi zadanou polozku,
// Vraci false pokud neexistuje zaznam
bool htab_remove(htab_t *t, const char *key);

// Zruseni vsech polozek, tabulka zustane prazdna
void htab_clear(htab_t *t);

// Rusi tabulku; vola htab_clear()
void htab_free(htab_t *t);

#endif
