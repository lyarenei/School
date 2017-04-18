// eratosthenes.c
// Řešení IJC-DU1, příklad a), 10.3.2017
// Autor: Dominik Křivohlávek, FIT
// Přeloženo: gcc 6.3.1
// Fuknce pro výpočet prvočísel pomocí Eratosthenova síta

#include <math.h>               // sqrt()
#include "bit_array.h"          // bitové pole a operace s ním

#ifndef ERATOSTHENES_C
#define ERATOSTHENES_C

void Eratosthenes(bit_array_t pole)
{
    // 0 a 1 nejsou prvočísla
    ba_set_bit(pole,0,1);
    ba_set_bit(pole,1,1);

    // Maximální počet iterací pro síto
    double maxval = sqrt(ba_size(pole));

    for(unsigned long i = 2; i < maxval; i++)
    {
        // Násobky prvočísel můžeme přeskočit
        if(ba_get_bit(pole,i) == 1)
            continue;

        // Násobky i nejsou prvočísla
        for(unsigned long j = 2*i; j < (ba_size(pole)); j += i)
            ba_set_bit(pole,j,1);
    }
}

#endif /* ERATOSTHENES_C */