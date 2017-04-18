// primes.c
// Řešení IJC-DU1, příklad a), 10.3.2017
// Autor: Dominik Křivohlávek, FIT
// Přeloženo: gcc 6.3.1
// Program pro výpočet prvočísel, vypisuje posledních 10 prvočísel z intervalu

#include <stdio.h>
#include <stdlib.h>             // EXIT_*
#include "bit_array.h"          // bitové pole a práce s ním
#include "error.h"              // chybové hlášky

void Eratosthenes(bit_array_t pole);

int main()
{
    // Vytvoření bitového pole a výpočet prvočísel
    ba_create(pole,303000000);
    Eratosthenes(pole);

    // Výpis posledních 10 prvočísel z intervalu
    int pocet = 0;
    for (unsigned long i = ba_size(pole)-1; i > 0; i--)
    {   
        if(ba_get_bit(pole,i) == 0)
        {
            pocet++;
        }
        if(pocet == 10)
        {
            for(; i < ba_size(pole)-1; i++)
            {
                if(ba_get_bit(pole,i)==0)
                {
                    printf("%lu\n",i);
                }
            }
            return EXIT_SUCCESS;
        }
    }
    return EXIT_FAILURE;
}
