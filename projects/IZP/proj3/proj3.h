/*
 * Kostra hlavickoveho souboru 3. projekt IZP 2015/16 a pro dokumentaci Javadoc.
 */

/*
 * autor: Dominik Křivohlávek (xkrivo02)
 * Prosinec 2016
 */

/**
 * @mainpage Obecne informace
 *
 * Toto je dokumenace ke tretimu projektu v ramci predmetu IZP pro ak. rok 2016
 * / 2017 na VUT v Brne.
 *
 * @section uvod O programu
 *
 * Cilem tohoto projektu bylo vytvorit program pro shlukovou analyzu
 * objektu.<br> Program provadi shlukovou analyzu dle metody nejvzdalenejsiho
 * souseda.<br> Analyzovane objekty jsou programu predany ve forme textoveho
 * souboru.
 *
 * @section spusteni Spusteni programu
 *
 * Program se spusti nasledujicim prikazem:<br>
 * @code{.unparsed}
 *  ./proj3 nazev_souboru N
 * @endcode
 *
 * nazev_souboru - soubor s objekty urcenymi k analyze,<br> N - konecny pocet
 * clusteru, do kterych maji byt objekty shlukovany
 *
 * @note       Pokud je program spusten bez parametru N, program implicitne
 *             shlukuje do jednoho clusteru.
 *
 * @section soubor_format Format souboru
 *
 * Na prvnim radku je nutne uvest pocet objektu ve formatu 'count=X', kde X je
 * pocet vsech objektu v souboru.<br> Kazdy objekt je pak samostatne na jednom
 * radku. Objekt je reprezentovan ve tvaru ID X Y, kde:<br> ID - jedinečný
 * identifikator objektu,<br> X - poloha objektu na ose X,<br> Y - poloha
 * objektu na ose Y.<br>
 *
 * @subsection ukazka_souboru Ukazka souboru s objekty
 * @code{.unparsed}
 * count=6
 * 40 86 663
 * 43 747 938
 * 47 285 973
 * 49 548 422
 * 52 741 541
 * 56 44 854
 * @endcode
 *
 * @note       Souradnice objektu vcetne jejich ID musi byt v celociselnem
 *             formatu. Program neakceptuje cisla s desetinnou carkou. <br>
 *             Souradnice take musi byt v intervalu 0 - 1000 (vcetne), ID
 *             objektu je omezeno maximalni velikosti celociselne promenne
 *             (obvykle 2^32 bitu).
 *
 * @section vystup Ukazka Vystupu
 *
 * @code{shell}
 * $ ./proj3 objekty 4
 * Clusters:
 * cluster 0: 40[86,663] 56[44,854]
 * cluster 1: 43[747,938]
 * cluster 2: 47[285,973]
 * cluster 3: 49[548,422] 52[741,541]
 * @endcode
 *
 * @code{bash}
 * $ ./proj3 objekty
 * Clusters:
 * cluster 0: 40[86,663] 43[747,938] 47[285,973] 49[548,422] 52[741,541] 56[44,854]
 * @endcode
 */

/**
 * @defgroup struktury Pouzivane struktury
 * @addtogroup struktury
 * @{
 */

/**
 * @brief      Struktura obj_t, obsahuje souradnice os X a Y vcetne identifikatoru
 */
struct obj_t {
    	/** Jedinecny identifikator objektu */
    int id;
    	/** X-ova souradnice objektu */
    float x;
    	/** Y-ova souradnice objektu */
    float y;
};


/**
 * @brief      Struktura cluster_t, obsahuje informaci o aktualni velikosti,
 *             kapacite a pole objektu
 *             
 * @file Proj3.h             
 */
struct cluster_t {
    	/** Aktualni velikost, tj. aktualni pocet objektu v clusteru */
    int size;
    	/** Maximalni kapacita clusteru, tj. kolik mista v pameti je pro tento
    	 * cluster alokovano
    	 */
    int capacity;
    	/** Ukazatel na pole objektu. Obsahuje vsechny objekty v clusteru */
    struct obj_t *obj;

};

/**
 * @}
 * @defgroup cluster_operace Operace nad clusterem
 * @addtogroup cluster_operace
 * @{
 * 
 */

/**
 * @brief      Inicializuje cluster. Alokuje pamet pro kapacitu objektu. V
 *             pripade chyby vypise chybovou hlasku s predanym ukazatelem.
 *
 * @param      c     Ukazatel na cluster.
 * @param      cap   Pozadovana kapacita.
 *
 * @pre        Existujici cluster.
 *
 * @post       Inicializovany cluster na zadanou hodnotu.
 */
void init_cluster(struct cluster_t *c, int cap);

/**
 * @brief      Uvolňuje cluster z pameti a inicializuje jej na cluster s
 *             kapacitou 0.
 *
 * @param      c     Ukazatel na cluster, ktery chceme uvolnit
 *
 * @pre        Existujici cluster
 *
 * @post       Cluster o nulove kapacite.
 */
void clear_cluster(struct cluster_t *c);

/**
 * Konstanta hodnoty 10. Pouzivana pro alokaci mista pro objekty
 */
extern const int CLUSTER_CHUNK;

/**
 * @brief      Realokuje pamet pro zadany cluster o novou velikost. Vraci NULL,
 *             pokud se realokovani nezdari. V pripade, ze pozadovana nova
 *             kapacita bude mensi nez aktualni kapacita, funkce vrati predany
 *             ukazatel na cluster zpet. Jinak funkce vraci ukazatel na pole o
 *             nove velikosti.
 *
 * @param      c        Ukazatel na cluster.
 * @param      new_cap  Pozadovana nova kapacita clusteru
 *
 * @return     Ukazatel na realokovane pole clusteru.
 *
 * @pre        Existujici cluster o nedostatecne kapacite.
 *
 * @post       Cluster o nove kapacite.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * @brief      Prida objekt do clusteru. V pripade chyby vypise chybovou hlasku
 *             vcetne predane adresy clusteru.
 *
 * @param      c     Adresa clusteru.
 * @param      obj   Pridavany objekt.
 *
 * @pre        Existujici cluster a objekt.
 *
 * @post       Objekt pridan na konec pole objektu v clusteru.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 * @brief      Spoji dva clustery do jednoho. V pripade chyby vypise chybovou
 *             hlasku vcetne predanych adres clusteru.
 *
 * @param      c1    Adresa ciloveho clusteru.
 * @param      c2    Adresa spojovaneho clusteru.
 *
 * @pre        Existujici 2 ruzne clustery.
 *
 * @post       Spojene clustery do jednoho.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @}
 * @defgroup pole_cluster_operace Operace nad polem clusteru
 * @addtogroup pole_cluster_operace
 * @{
 */

/**
 * @brief      Odstrani cluster z pole na konkretnim indexu. Funkce vraci novy
 *             pocet prvku v poli. V pripade chyby vraci hodnotu -1 a vypisuje
 *             prislusnou chybovou hlasku.
 *
 * @param      carr  Adresa pole clusteru.
 * @param      narr  Pocet clusteru (indexu) v poli clusteru.
 * @param      idx   Index v poli, na kterem chceme cluster vymazat.
 *
 * @pre        Existujici pole clusteru o veliksoti narr.
 *
 * @return     Pocet prvku v poli.
 *
 * @post       Pole zmensene o jeden cluster.
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
 * @brief      Pocita vzdalenost mezi dvema objekty. Funkce vraci hodnotu -1 pri
 *             chybnem vypoctu.
 *
 * @param      o1    Prvni objekt.
 * @param      o2    Druhy objekt.
 *
 * @return     Euklidovska vzdalenost mezi objekty.
 * 
 * @pre        Existujici 2 ruzne objekty.
 * 
 * @post       Vypocitana vzdalenost mezi objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 * @brief      Pocita vzdalenost mezi dvema clustery, podle metody
 *             nejvzdalenejsiho souseda. Funkce vraci hodnotu -1 v pripade
 *             chyby.
 *
 * @param      c1    Prvni cluster.
 * @param      c2    Druhy cluster.
 *
 * @return     Vzdalenost mezi clustery.
 * 
 * @pre        Existujici 2 ruzne clustery.
 * 
 * @post       Vypoctena vzdalenost mezi clustery.
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @brief      Funkce nalezne dva mezi sebou nejblizsi clustery. Funkce tyto
 *             clustery uklada na ukazatele c1 a c2. V pripade chyby zapise na
 *             tyto ukazatele hodnoty -1.
 *
 * @param      carr  Adresa pole clusteru.
 * @param      narr  Velikost (pocet indexu) pole clusteru.
 * @param      c1    Adresa promenne pro jeden index pole clusteru.
 * @param      c2    Adresa promenne pro druhy index poel clusteru.
 *
 * @pre        Existujici pole clusteru o velikosti narr.
 *
 * @post       Indexy nalezenych dvou nejblizsich clusteru mezi sebou, zapasane
 *             na adresy c1 a c2.
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 * @brief      Setridi pole objektu v clusteru podle jejich identifikatoru
 *             (vzestupne).
 *
 * @param      c     Adresa trideneho clusteru.
 * 
 * @pre        Existujici cluster.
 * 
 * @post       Serazene objekty v clusteru dle jejich ID vzestupne.
 */
void sort_cluster(struct cluster_t *c);

/**
 * @brief      Tiskne vsechny objekty z clusteru na standardni vystup.
 *
 * @param      c     Adresa tisknuteho clusteru.
 * 
 * @pre        Existujici cluster.
 * 
 * @post       Vytiskle objekty clusteru na standardnim vystupu.
 */
void print_cluster(struct cluster_t *c);

/**
 * @brief      Nacita objekty ze zdrojoveho souboru.
 *
 * @param      filename  Nazev zdrojoveho souboru.
 * @param      arr       Ukazatel na adresu pole clusteru, do ktereho se
 *                       zapisuji objekty.
 *
 * @return     Adresa alokovaneho pole clusteru s nactenymi objekty.
 * 
 * @pre        Existujici otevirany soubor a ukazatel na pole clusteru.
 * 
 * @post       Nactene objekty v clusterech. 			
 */
int load_clusters(char *filename, struct cluster_t **arr);

/**
 * @brief      Tiskne vsechny clustery ze zadaneho pole clusteru na standardni
 *             vystup.
 *
 * @param      carr  Adresa tisknuteho pole clusteru.
 * @param      narr  Velikost (pocet indexu) tisknuteho clusteru.
 * 
 * @pre        Existujici pole clusteru o velikosti narr.
 * 
 * @post       Vytiskle clustery na standardnim vystupu.
 */
void print_clusters(struct cluster_t *carr, int narr);

/**
 * @brief      Uvolnuje alokovanou pamet pro pole clusteru.
 *
 * @param      arr   Pole uvolnovaneho clusteru.
 * @param      narr  Velikost uvolnovaneho clusteru.
 * 
 * @pre        Existujici pole clusteru.
 * 
 * @post       Uvolnena alokovana pamet pro pole clusteru.
 */
void free_clusters(struct cluster_t *arr, int narr);
/**
 * @}
 */