// hash_function.c
// Řešení IJC-DU2, příklad b), 30.3.2017
// Autor: Dominik Křivohlávek, FIT
// Přeloženo: gcc 6.3.1
// Rozptylovaci funkce potrebna po vypocet indexu v hash tabulce

#ifndef HASH_FUNCTION
#define HASH_FUNCTION

unsigned int hash_function(const char *str)
{
    unsigned int h = 0;
    const unsigned char *p;
    for(p=(const unsigned char*)str; *p!='\0'; p++)
    h = 65599*h + *p;
    return h;
}

#endif /* HASH_FUNCTION */
