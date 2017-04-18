// ppm.h
// Řešení IJC-DU1, příklad b), 10.3.2017
// Autor: Dominik Křivohlávek, FIT
// Přeloženo: gcc 6.3.1
// Definice struktury ppm a funkcí pro práci se strukturou

#ifndef PPM_H
#define PPM_H

struct ppm 
{
    unsigned xsize;
    unsigned ysize;
    char data[];    // RGB bajty, celkem 3*xsize*ysize
};

// Načte obsah PPM souboru struktury ppm
struct ppm * ppm_read(const char * filename);

// Zapíše obsah struktury p do souboru ve formátu PPM.
int ppm_write(struct ppm *p, const char * filename);

#endif /* PPM_H */