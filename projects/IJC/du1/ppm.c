// ppm.c
// Řešení IJC-DU1, příklad b), 10.3.2017
// Autor: Dominik Křivohlávek, FIT
// Přeloženo: gcc 6.3.1
// Funkce pro práci se sktukturou ppm

#include <stdio.h>
#include <stdlib.h>             // malloc()
#include <string.h>             // strcmp()
#include "error.h"              // chybové hlášky
#include "ppm.h"                // struktura ppm a prototypy funkcí


// Načte obsah PPM souboru struktury ppm
struct ppm * ppm_read(const char * filename)
{
    FILE *data;

    if((data = fopen(filename, "rb")) == NULL)
    {
        warning_msg("Soubor '%s' se nepodařilo otevřít!\n", filename);
        return NULL;
    }

    char buffer[10] = {0};

    // Načtení verze PPM
    fread(buffer,sizeof(char),2,data);
    if(strcmp(buffer, "P6"))
    {
        warning_msg("Soubor '%s' má nesprávný formát! (Verze PPM)\n", filename);
        return NULL;
    }
    
    // Načtení rozměrů obrázku
    unsigned x_size = 0;
    unsigned y_size = 0;

    fread(buffer, sizeof(unsigned),2,data);
    if(sscanf(buffer, "%u %u", &x_size, &y_size) != 2)
    {
        printf("Obsah bufferu: %s\n",buffer);
        warning_msg("Soubor '%s' má chybné rozměry!", filename);
        return NULL;
    }
    if((x_size > 1000) || (y_size > 1000))
    {
        warning_msg("Zpracovávaný obrázek je příliš velký!\n");
        return NULL;
    }

    // Vynechání informace o barevné hloubce
    fread(buffer,sizeof(unsigned),1,data);
    
    // Alokace místa pro rozměry a obrazová data
    struct ppm *picture = malloc((2*sizeof(unsigned))+(3*x_size*y_size)+1);

    if(picture == NULL)
    {
        warning_msg("Nezdařilo se alokovat potřebné místo v paměti!\n");
        return NULL;
    }
        
    // Zápis informací o rozměru obrázku
    picture->xsize = x_size;
    picture->ysize = y_size;

    // Načtení RGB dat obrázku
    fread(&(picture->data),sizeof(char),(3*x_size*y_size)+1,data);

    if(fclose(data) == EOF)
    {
        warning_msg("Soubor '%s' se nepodařilo uzavřít\n", filename);
        free(picture);
        return NULL;
    }
    return picture;
}

// Zapíše obsah struktury p do souboru ve formátu PPM.
int ppm_write(struct ppm *p, const char * filename)
{
    if(p == NULL)
    {
        warning_msg("Nelze zapsat obsah struktury do souboru!\n");
        return -1;
    }

    FILE *data;

    if((data = fopen(filename,"ab")) == NULL)
    {
        warning_msg("Nezdařilo se otevřít / vytvořit soubor pro zápis!\n");
        return -1;
    }

    // Zápis verze PPM, při max. rozměrech obrázku 1000x1000 nepřesáhne celková délka 20 znaků
    char buffer[20];
    snprintf(buffer,20,"P6\n%d %d\n255",(p->xsize),(p->ysize));
    fwrite(buffer,strlen(buffer),1,data);
    
    // Zápis RGB bajtů do souboru
    fwrite((p->data),sizeof(char),(3*p->xsize*p->ysize)+1,data);
    
    if(fclose(data) == EOF)
    {
        warning_msg("Soubor '%s' se nepodařilo uzavřít\n", filename);
        return -1;
    }
    return 0;
}