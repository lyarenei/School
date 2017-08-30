// tail.c
// Řešení IJC-DU2, příklad a), 30.3.2017
// Autor: Dominik Křivohlávek, FIT
// Přeloženo: gcc 6.3.1
// Program vypisujici poslednich 10 radku ze souboru / stdin

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Alokuje potrebne misto pro buffer
// Vraci 1 v pripade chyby, jinak 0
int buffer_create(char **buffer, unsigned size)
{
    if(buffer == NULL)
    {
        printf("Byl predan neplatny ukazatel!\n");
        return 1;
    }
    // Alokace mista pro 'stringy'
    for(unsigned i = 0; i < size; i++)
    {
        // Omezeni na 1024 +1 char pro '\0'
        buffer[i] = malloc(1026*sizeof(char));

        if(buffer[i] == NULL)
        {
            fprintf(stderr,"Nezdarilo se alokovat potrebne misto!\n");
            return 1;
        }

        // Nulovani retezce
        memset(buffer[i], 0, 1026);
    }
    return 0;
}

// Dealokuje pamet pro buffer
// Vraci 1 v pripade chyby, jinak 0
int buffer_delete(char **buffer, unsigned size)
{
    if(buffer == NULL)
    {
        printf("Byl predan neplatny ukazatel!\n");
        return 1;
    }

    for(unsigned i = 0; i < size; i++)
    {
        free(buffer[i]);
        buffer[i] = NULL;
    }
    return 0;
}

// Vypisuje poslednich n radku ze vstupu
void print_tail(FILE *data, int size)
{
    // Hlavni buffer, alokace pameti, slouzi pro zachyt a vypis radku
    char *buffer[size];
    buffer_create(buffer,size);

    // Pomocne promenne pro nacitany znak a pozici na radce
    int c;
    unsigned pos = 0;
    char line[1026];        // 1024 + \n + \0

    // Uzivatel ma byt varovan pouze jednou
    int warned = 0;

    while((c = fgetc(data)) != EOF)
    {   
        // Rozsah je 0 - 1023 znaku
        if(pos > 1023)
        {
            if(warned == 0)
            {
                fprintf(stderr,"Byla prekrocena maximalni delka radku!\n");
                warned = 1;
            }
            line[pos] = '\n';
            line[pos+1] = '\0';

            // Skip zbyvajicich znaku ze stdin
            while((c = fgetc(data)) != '\n');

            // Posunuti radku v bufferu
            for(unsigned i = size-1; i > 0; i--)
                strcpy(buffer[i],buffer[i-1]);
            
            strcpy(buffer[0],line);           

            pos=0;
            continue;
        }

        // Konec jednoho 'stringu'; pridani '\0' na konec
        if(c == '\n')
        {
            line[pos] = c;
            line[pos+1] = '\0';
            pos=0;

            // Posunuti radku v bufferu
            for(unsigned i = size-1; i > 0; i--)
                strcpy(buffer[i],buffer[i-1]);
            
            strcpy(buffer[0],line);
            continue;
        }
        line[pos] = c;
        pos++;
    }

    // Vypis bufferu
    for(int i=size-1; i >= 0; i--)
        printf("%s",buffer[i]);
    
    buffer_delete(buffer,size);
}

int main(int argc, char **argv)
{
    int line_cnt = 10;
    
    // Kontrola argumentu
    if (argc > 1) 
    {
        for(int i = 1; i < argc; i++)
        {
            if(!strcmp(argv[i],"-n")) 
            {
                if(i+1 == argc)
                {
                    fprintf(stderr, "Zadan neplatny pocet radku!\n");
                    return EXIT_FAILURE;
                }
                else if(1 == sscanf(argv[i+1],"%d",&line_cnt))
                {
                    if(line_cnt == 0)
                        return EXIT_SUCCESS;
                    
                    if(line_cnt < 0)
                    {
                        fprintf(stderr, "Zadan neplatny pocet radku!\n");
                        return EXIT_FAILURE;
                    }

                    // Posun v argv
                    i++;
                }
                else
                {
                    fprintf(stderr, "Zadan neplatny pocet radku!\n");
                    return EXIT_FAILURE;
                }
            }
            else 
            {
                FILE *data;
                data = fopen(argv[i],"r");
                if (NULL == data) 
                {
                    fprintf(stderr, "Nezdarilo se otevrit soubor '%s'\n", argv[i]);
                    return EXIT_FAILURE;
                }

                // Vypis radku
                print_tail(data, line_cnt);
                
                // Uzavreni souboru
                if(fclose(data) == EOF)
                {
                    fprintf(stderr, "Nezdarilo se uzavrit soubor '%s'\n", argv[i]);
                    return EXIT_FAILURE;
                }
                return EXIT_SUCCESS;
            }
        }
        print_tail(stdin, line_cnt);
        return EXIT_SUCCESS;
    } 
    else 
    {
        print_tail(stdin, line_cnt);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;   
}
