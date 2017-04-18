// steg-decode.c
// Řešení IJC-DU1, příklad b), 10.3.2017
// Autor: Dominik Křivohlávek, FIT
// Přeloženo: gcc 6.3.1
// Program pro dešifrování zprávy z obrázku ve formátu PPM

#include <stdio.h>
#include <stdlib.h>             // EXIT_*
#include <limits.h>             // CHAR_BIT
#include "bit_array.h"          // bitove pole a operace s ním
#include "error.h"              // chybové hlášky
#include "ppm.h"                // struktura PPM a prototypy funkcí

void Eratosthenes(bit_array_t pole);

int main(int argc, char **argv)
{
    // Ošetření argumentů
    if(argc > 2)
    {
        warning_msg("Byly zadany neplatne argumenty!\n");
        return EXIT_FAILURE;
    }
    struct ppm *picture = ppm_read(argv[1]);

    if(picture == NULL)
    {
        warning_msg("Nepodařilo se zpracovat otevíraný soubor\n");
        return EXIT_FAILURE;
    }

    // Výpočet prvočísel
    ba_create(pole,3*1000*1000);
    Eratosthenes(pole);
    
    // Buffer pro sestavení znaku
    char znak[] = {0};
    
    // Výpis zprávy
    for(unsigned long i = 0; i < ba_size(pole);)
    {
        for(int j = 0; j < 8;)
        {
            if(i >= (ba_size(pole)))
                error_msg("Zpráva není korektně ukončena!\n");
            
            if(ba_get_bit(pole,i) == 0)
            {
                BA_SET_BIT_(znak,j,BA_GET_BIT_(picture->data,(i+1)*CHAR_BIT));
                j++;
            }            
            i++;
        }
        if(znak[0] == '\0')
        {
            printf("\n%c",znak[0]);
            free(picture);
            return EXIT_SUCCESS;
        }
        else
            printf("%c",znak[0]);
    }
    free(picture);
    return EXIT_FAILURE;
}