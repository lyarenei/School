/**
 * Kostra programu pro 3. projekt IZP 2015/16
 *
 * Jednoducha shlukova analyza
 * Complete linkage
 * http://is.muni.cz/th/172767/fi_b/5739129/web/web/clsrov.html
 * 
 * Autor doplněných těl funkcí: Dominik Křivohlávek (xkrivo02)
 * Listopad 2016
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <string.h>

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
--------------------------------------------------------------------
 Funkce vypise hlasku v pripade chyby.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    if(c == NULL)
    {
        fprintf(stderr, "Nelze incializovat cluster na adrese: %p\n", (void *)c);
        return;
    }
    if(cap <= 0)
    {
        c->size = 0;
        c->capacity = 0;
        c->obj = NULL;
    }
    else
    {
        c->size = 0;
        c->capacity = cap;
        c->obj = malloc(cap*sizeof(struct obj_t));
    }
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    if(c != NULL)
    {      
        free(c->obj);
        init_cluster(c, 0);
    }
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
--------------------------------------------------------------------
 Funkce vypise hlasku v pripade chyby.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    if(c == NULL)
    {
        fprintf(stderr, "Nelze pridat objekt do clusteru na adrese: %p\n", (void *)c);
        return;
    }
    if(c->size < c->capacity)
    {
        c->obj[c->size].id = obj.id;
        c->obj[c->size].x = obj.x;
        c->obj[c->size].y = obj.y;
        c->size++;
    }
    else
    {
        c = resize_cluster(c, (c->size + CLUSTER_CHUNK));
        c->obj[c->size].id = obj.id;
        c->obj[c->size].x = obj.x;
        c->obj[c->size].y = obj.y;
        c->size++;
    }
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
--------------------------------------------------------------------
 Funkce vypise hlasku v pripade chyby.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    if(c1 == NULL || c2 == NULL)
    {
        fprintf(stderr, "Nelze spojit shluky! (Adresy: %p, %p)\n", (void *)c1, (void *)c2);
        return;
    }

    // Iterace pro vsechny objekty ve zdrojovem clusteru
    for(int i = 0; i < c2->size; i++)
    {
        append_cluster(c1, c2->obj[i]);
    }

    sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
--------------------------------------------------------------------
 Funkce vraci hodnotu -1 v pripade jakekoli chyby.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    if(carr == NULL)
        return -1;

    if(narr < 1 || narr >= INFINITY)
    {
        fprintf(stderr, "Spatna velikost pole! (%d)\n", narr);
        return -1;
    }

    if(idx < 0 || idx >= INFINITY)
    {
        fprintf(stderr, "Zadany index je mimo pole! (%d)\n", idx);
        return -1;
    }

    // Iterace na poli clusteru
    for(int i = 0; i < narr; i++)
    {    // Kontrola na cluster, ktery chceme vymazat    
        if(i == idx)
        {          
            clear_cluster(&carr[i]);                             // Reset mazaneho clusteru
            merge_clusters(&carr[i], &carr[narr -1]);            // Zkopirovani posledniho clusteru na resetovany cluster
            clear_cluster(&carr[narr -1]);                       // Reset kopirovaneho clusteru
            break;
        }
    }                 
    return --narr;              // Zmenseni 'platne' velikosti pole clusteru
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
--------------------------------------------------------------------
 Funkce vraci -1 v pripade chyby.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    if(o1 == NULL || o2 == NULL)
        return -1;

    // Vzorec pro vypocet vzdalenosti mezi dvema body
    float distance = -1;
    distance = sqrtf(pow((o1->x)-(o2->x),2)+pow((o1->y)-(o2->y),2));

    if(distance < 0 || distance >= INFINITY)
        return -1;

    return distance;
}

/*
 Pocita vzdalenost dvou shluku.
--------------------------------------------------------------------
 Funkce vraci -1 v pripade chyby.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    if(c1 == NULL || c2 == NULL)
        return -1;

    // Deklarace a inicializace vzdalenosti
    float distance = 0;
    float max_distance = distance = obj_distance(&c1->obj[0], &c2->obj[0]);

    if(distance < 0)
        return -1;

    // Iterace na objektech v prvnim clusteru
    for(int i = 0; i < c1->size; i++)
    {   // Iterace na objektech ve druhem clusteru
        for(int j = 0; j < c2->size; j++)
        {
            distance = obj_distance(&c1->obj[i], &c2->obj[j]);
            if(distance < 0)
                return -1;

            if(distance > max_distance)
                max_distance = distance;
        }
    }
    return max_distance;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
--------------------------------------------------------------------
 Funkce zapisuje na adresy indexu hodnotu -1, a pripadne vypise chybovou hlasku.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

    // Osetreni parametru
    if(carr == NULL)
    {
        *c1 = -1;
        *c2 = -1;
        return;
    }

    if(narr < 1)
    {
        fprintf(stderr, "Spatna velikost pole! (Velikost: %d)\n", narr);
        *c1 = -1;
        *c2 = -1;
        return;
    }

    // Deklarace a inicializace vzdalenosti
    float distance = 0;
    float min_distance = distance = cluster_distance(&carr[0], &carr[narr-1]);

    if(distance < 0)
    {
        fprintf(stderr, "Spatna vzdalenost clusteru! (Vzdalenost: %f)\n", distance);
        *c1 = -1;
        *c2 = -1;
        return;
    }

    // Iterace na poli clusteru od zacatku
    for(int i = 0; i < (narr -1); i++)
    {   // Iterace na poli shluku od konce
        for(int j = (narr-1); j > 0; j--)
        {   // Abychom nepocitali vzdalenost mezi stejnymi shluky (nula)
            if(i != j)          
            {
                if((distance = cluster_distance(&carr[i], &carr[j])) < 0)
                {
                    fprintf(stderr, "Spatna vzdalenost clusteru! (Vzdalenost: %f)\n", distance);
                    *c1 = -1;
                    *c2 = -1;
                    return;
                }              
                if(distance <= min_distance)
                {
                    min_distance = distance;
                    *c1 = i;
                    *c2 = j;
                }
            }
        }
    }
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = a;
    const struct obj_t *o2 = b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/* Uvolni vsechny clustery z pole arr o velikosti narr */
void free_clusters(struct cluster_t *arr, int narr)
{
    for(int i = 0; i < narr; i++)
    {           // Uvolneni alokovanych clusteru
        clear_cluster(&arr[i]);
    }
}

/* 
 Specificke pro funkci load_clusters, vypisuje chybovou hlasku a uvolnuje pomocne promenne.
 Tiskne chybovy retezec 'text'
 Uvolnuje z pameti pole cluster 'clusters'
 Uvolnuje z pameti pomocnou strukturu 'object' typy obj_t
 Inicializace zbyvajicich clusteru od indexu 'read_num'
 Velikost pole clusteru - 'number'
 */ 
void print_load_err(const char *text, struct cluster_t **arr, struct cluster_t *clusters, struct obj_t *object, int read_num, int number)
{
    fprintf(stderr, "%s\n", text);
    for(int i = read_num; i < number; i++)          // Inicializace nevyuzitych clusteru po detekci chyby
        init_cluster(&clusters[i], 0);
    
    free_clusters(clusters, number);
    *arr = clusters;
    free(object);               
    object = NULL;
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);

    int number = 0;         // Pocet ocekavanych objektu
    int read_num = 0;       // Pocet prectenych objektu
    struct cluster_t *clusters = NULL;
    struct obj_t *object = NULL;
    char tmp[500];          // Pomocne pole pro parsovani radku ze souboru
    
    // Pomocne promenne pro kontrolu celociselnych hodnot
    int id = 0;
    int x_coor = 0;
    int y_coor = 0;

    FILE *data; 
    if((data = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "Soubor se nepodarilo otevrit! (Soubor: %s)\n", filename);
        return 0;
    }

    // Nacteni informace o poctu clusteru
    if(fgets(tmp, sizeof(tmp), data) != NULL)
    {           
        if(strncmp(tmp, "count=", 6))
        {
            fprintf(stderr, "Chyba ve formatu vstupniho souboru!\n");
            if(fclose(data) == EOF)
                fprintf(stderr, "Soubor se nepodarilo uzavrit! (Soubor: %s)\n", filename);
            return 0;
        }
        sscanf(tmp, "%*[^0-9[]" "%d", &number);         // Nacteni poctu objektu za retezcem "clusters="
    }

    // Test platneho poctu objektu v souboru
    if(number <= 0)
    {
        fprintf(stderr, "Chyba ve formatu vstupniho souboru!\n");
        if(fclose(data) == EOF)
            fprintf(stderr, "Soubor se nepodarilo uzavrit! (Soubor: %s)\n", filename);
        return read_num;
    }
    else
    {
        // Alokace pole clusteru
        clusters = malloc(number*sizeof(struct cluster_t));

        // Alokace pomocneho objektu
        object = malloc(sizeof(struct obj_t));

        // Pomocne pole identifikatoru a jeho inicializace
        int arr_id[number];
        for(int i = 0; i < number; i++)
            arr_id[i] = -1;

        // Indexace arr_id
        int i = 0;

        // Nacitani dat ze souboru po radcich
        while (fgets(tmp, sizeof tmp, data) != NULL && read_num < number)
        {   // Test nactenych dat na typ int
            if(sscanf(tmp, "%d %d %d %s", &id, &x_coor, &y_coor, tmp) == 3)
            {
                for(int j = 0; j < number; j++)
                {
                    if(id == arr_id[j])
                    {
                        print_load_err("Zjistena duplicita identifikatoru objektu!", arr, clusters, object, read_num, number);
                        if(fclose(data) == EOF)
                            fprintf(stderr, "Soubor se nepodarilo uzavrit! (Soubor: %s)\n", filename);
                        return 0;
                    }
                }
                arr_id[i] = id;
                i++;

                // Test nactenych cisel na pozadovane intervaly
                if(id < INT_MAX && x_coor <= 1000 && y_coor <= 1000 && x_coor >= 0 && y_coor >= 0)
                {
                    object->id = id;
                    object->x = x_coor;
                    object->y = y_coor;
                    init_cluster(&clusters[read_num], CLUSTER_CHUNK);
                    append_cluster(&clusters[read_num], *object);
                    read_num++;
                }
                else
                {
                    print_load_err("Chyba ve formatu vstupniho souboru!", arr, clusters, object, read_num, number);
                    if(fclose(data) == EOF)
                        fprintf(stderr, "Soubor se nepodarilo uzavrit! (Soubor: %s)\n", filename);
                    return 0;   
                }
            }
            else
            {
                print_load_err("Chyba ve formatu vstupniho souboru!", arr, clusters, object, read_num, number);
                if(fclose(data) == EOF)
                    fprintf(stderr, "Soubor se nepodarilo uzavrit! (Soubor: %s)\n", filename);
                return 0;
            }
        }
    }
    if(read_num != number)
    {
        print_load_err("Chyba ve formatu vstupniho souboru!", arr, clusters, object, read_num, number);
        if(fclose(data) == EOF)
            fprintf(stderr, "Soubor se nepodarilo uzavrit! (Soubor: %s)\n", filename);  
        return 0;
    }
    if(fclose(data) == EOF)
        fprintf(stderr, "Soubor se nepodarilo uzavrit! (Soubor: %s)\n", filename);

    *arr = clusters;            // Predame zpet adresu naalokovaných clusteru
    free(object);               
    object = NULL;
    return read_num;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[])
{
    if(argc < 2 || argc > 3)
    {
        fprintf(stderr, "Prilis malo/mnoho argumentu!\n");
        return EXIT_FAILURE;
    }
    char *err = NULL;           // Ukazatel na char na vstup strtol( bude ukazovat na zbytek retezece v pripade chybneho vstupu)
    struct cluster_t *clusters = NULL;
    int index1 = 0;
    int index2 = 0;
    int c_out = 0;              // Pozadovany pocet clusteru na vystupu
    if(argc == 2)
        c_out = 1;
    else
    {
        c_out = (int)strtol(argv[2], &err, 10);
        if(*err != '\0' || c_out <= 0)
        {
            fprintf(stderr, "Nebyl zadan platny pocet clusteru!\n");
            return EXIT_FAILURE;             // Kontrola na spravnou konverzi znaku na ciselnou hodnotu a na zapornou hodnotu
        }
    }
    int number = load_clusters(argv[1], &clusters);          // Pocet pocatecnich clusteru
    int narr = number;              // Pocet clusteru v poli clusteru
    if(number <= 0)
    {
        free(clusters);            
        clusters = NULL;
        return EXIT_FAILURE;
    }
    if(number < c_out)
    {
        fprintf(stderr, "Prilis malo objektu v souboru! (Soubor: %s)\n", argv[1]);
        free_clusters(clusters, narr);
        free(clusters);            
        clusters = NULL;
        return EXIT_FAILURE;
    }
    if(clusters != NULL)
    {
        for(int i = 0; i < number - c_out; i++)
        {
            find_neighbours(clusters, narr, &index1, &index2);
            if(index1 < 0 || index2 < 0)
            {
                fprintf(stderr, "Chyba pri pocitani sousednich clusteru! (Pole: %p)\n", (void *)clusters);
                free_clusters(clusters, narr);
                return EXIT_FAILURE;
            }
            merge_clusters(&clusters[index2], &clusters[index1]);
            narr = remove_cluster(clusters, narr, index1);
            if(narr < 0)
            {
                fprintf(stderr, "Nespravna velikost pole po zmenseni! (Velikost: %d)\n", narr);
                free_clusters(clusters, narr);
                return EXIT_FAILURE;
            }
        }
        print_clusters(clusters, narr);
    }
    free_clusters(clusters, narr);
    free(clusters);            
    clusters = NULL;
    return EXIT_SUCCESS;
}