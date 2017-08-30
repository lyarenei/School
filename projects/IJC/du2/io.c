// io.c
// Řešení IJC-DU2, příklad b), 30.3.2017
// Autor: Dominik Křivohlávek, FIT
// Přeloženo: gcc 6.3.1
// Modul s funkci get_word...

#include <stdio.h>
#include <ctype.h>

#ifndef GET_WORD
#define GET_WORD

/*  
Funkce cte jedno slovo ze souboru f do zadaneho
pole znaku a vraci delku slova. Pokud je slovo
delsi, nez 'max', nacita 'max' - 1 znaku a zbytek
preskoci. Vraci EOF pokud je dosazeno konce souboru.
*/

int get_word(char *s, int max, FILE *f)
{
    if(s == NULL)
    {
        fprintf(stderr, "Byl predan nespravny ukazatel na pole!\n");
        return -1;
    }

    int warning = 0;
    int count = 0;
    int c;

    // Nacitani znaku
    while((c = fgetc(f)) != EOF)
    {
        // Kontrola znaku na mezeru
		if(isspace(c))
		{
			s[count] = '\0';
			return count;
		}

        // Rozsah slova 0 - 126 = 127 znaku
        if(count > max-1)
        {
            if(warning == 0)
            {
                fprintf(stderr,"Byla prekrocena maximalni delka slova!\n");
                warning = 1;
            }
			s[count] = '\0';

            // Preskoceni dalsich znaku
            while ((c = fgetc(f)) != EOF)
            {
				if(isspace(c))
					break;
            }
            return count;
        }
        s[count] = c;
        count++;
    }
    s[count] = '\0';
    if(c == EOF)
        return EOF;
    
    return count;
}

#endif /* GET_WORD */
