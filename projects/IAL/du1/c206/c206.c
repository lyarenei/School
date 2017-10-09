
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, říjen 2017
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu, 
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek, 
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku, 
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu, 
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem, 
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu, 
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError()
{
    /*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/
    printf("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE; /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList(tDLList *L)
{
    /*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
    if (L == NULL)
        return;

    L->First = NULL;
    L->Act = NULL;
    L->Last = NULL;
}

void DLDisposeList(tDLList *L)
{
    /*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/
    if (L == NULL)
        return;
    else if (L->First == NULL)
        return;
    else
    {
        // Uložení ukazatele k posunu po seznamu
        struct tDLElem *tmp = L->First->rptr;
        while (tmp != NULL)
        {
            if (tmp->lptr != NULL) // Ošetření proti uvolnění prvního prvku
                free(tmp->lptr);
            tmp->lptr = NULL;
            tmp = tmp->rptr;
        }
        // Uvolnění posledního prvku, který je prvním prvkem v listu
        free(L->First);

        // Inicializace vymazaného listu
        DLInitList(L);
    }
}

void DLInsertFirst(tDLList *L, int val)
{
    /*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    if (L == NULL)
        return;

    struct tDLElem *tmp = malloc(sizeof(struct tDLElem));
    if (tmp == NULL)
    {
        DLError();
        return;
    }

    // Zápis dat
    tmp->data = val;
    tmp->lptr = NULL;
    tmp->rptr = NULL;

    /* Kontrola stavu seznamu, pokud je seznam prázdný,
       položka je prvním i posledním prvkem v listu */
    if (L->First == NULL)
    {
        L->First = tmp;
        L->Last = tmp;
    }
    else
    {
        tmp->rptr = L->First; // Zápis následníka
        L->First->lptr = tmp; // Zápis nového předka původního prvního prvku
        L->First = tmp;       // Zápis nového prvního prvku do listu
    }
}

void DLInsertLast(tDLList *L, int val)
{
    /*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    struct tDLElem *tmp = malloc(sizeof(struct tDLElem));
    if (tmp == NULL)
    {
        DLError();
        return;
    }
    // Zápis dat
    tmp->data = val;
    tmp->lptr = NULL;
    tmp->rptr = NULL;

    /* Kontrola stavu seznamu, pokud je seznam prázdný,
       položka je prvním i posledním prvkem v listu */
    if (L->Last == NULL)
    {
        L->First = tmp;
        L->Last = tmp;
    }
    else
    {
        tmp->lptr = L->Last; // Zápis předka
        L->Last->rptr = tmp; // Zápis nového následníka původního posledního prvku
        L->Last = tmp;       // Zápis nového posledního prvku do listu
    }
}

void DLFirst(tDLList *L)
{
    /*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
    L->Act = L->First;
}

void DLLast(tDLList *L)
{
    /*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
    L->Act = L->Last;
}

void DLCopyFirst(tDLList *L, int *val)
{
    /*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
    if (L == NULL)
        return;
    else if (L->First == NULL)
    {
        DLError();
        return;
    }
    else
        *val = L->First->data;
}

void DLCopyLast(tDLList *L, int *val)
{
    /*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
    if (L == NULL) // Ošetření ukazatele
        return;

    else if (L->Last == NULL) // Ošetření prázdnosti
    {
        DLError();
        return;
    }
    else
        *val = L->Last->data;
}

void DLDeleteFirst(tDLList *L)
{
    /*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
    if (L == NULL) // Ošetření ukazatele
        return;
    else if (L->First == NULL || L->Last == NULL) // Ošetření prázdnosti
        return;
    else if (L->Act == L->Last) // Zrušení aktivity
        L->Act = NULL;
    else
    {
        struct tDLElem *tmp = L->First->rptr; // Uložení nového posledního prvku
        tmp->lptr = NULL;                     // První prvek nemá předka

        free(L->First);
        L->First = tmp; // Zápis nového prvního prvku listu
    }
}

void DLDeleteLast(tDLList *L)
{
    /*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
    if (L == NULL) // Ošetření ukazatele
        return;
    else if (L->First == NULL || L->Last == NULL) // Ošetření prázdnosti
        return;
    else if (L->Act == L->Last) // Zrušení aktivity
        L->Act = NULL;
    else
    {
        struct tDLElem *tmp = L->Last->lptr; // Uložení nového posledního prvku
        tmp->rptr = NULL;                    // Poslední prvek nemá následníka

        free(L->Last);
        L->Last = tmp; // Zápis nového posledního prvku listu
    }
}

void DLPostDelete(tDLList *L)
{
    /*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
    if (L == NULL) // Ošetření ukazatele
        return;
    else if (DLActive(L) == 0 || L->Act == L->Last) // Ošetření aktivity a kontrola aktivního prvku
        return;
    else
    {
        struct tDLElem *tmp = L->Act->rptr->rptr; // Uložení nového následníka Act
        free(L->Act->rptr);                       // Uvolnení následníka Act
        L->Act->rptr = NULL;

        // Pokud následník uvolněného následníka Act neexistoval...
        if (tmp != NULL)
            L->Act->rptr = tmp;
        else
            L->Last = L->Act; //.. zapíšeme jej také do listu jako poslední prvek
    }
}

void DLPreDelete(tDLList *L)
{
    /*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
    if (L == NULL) // Ošetření ukazatele
        return;
    else if (DLActive(L) == 0 || L->Act == L->First) // Ošetření aktivity a kontrola aktivního prvku
        return;
    else if (L->Act->lptr == NULL)
        return;
    else
    {
        struct tDLElem *tmp = L->Act->lptr->lptr; // Uložení nového předka Act
        free(L->Act->lptr);                       // Uvolnení předka Act
        L->Act->lptr = NULL;

        // Pokud předek uvolněného předka Act neexistoval...
        if (tmp != NULL)
            L->Act->lptr = tmp;
        else
            L->First = L->Act; //.. zapíšeme jej také do listu jako první prvek
    }
}

void DLPostInsert(tDLList *L, int val)
{
    /*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    if (L == NULL) // Ošetření ukazatele
        return;
    else if (DLActive(L) == 0) // Ošetření aktivity
        return;
    else
    {
        struct tDLElem *tmp = malloc(sizeof(struct tDLElem));
        if (tmp == NULL)
        {
            DLError();
            return;
        }
        tmp->data = val;

        tmp->lptr = L->Act;       // Zápis předka
        tmp->rptr = L->Act->rptr; // Zápis následníka

        L->Act->rptr->lptr = tmp; // Zápis nového předka do původního následníka Act
        L->Act->rptr = tmp;       // Zápis nového následníka do Act
    }
}

void DLPreInsert(tDLList *L, int val)
{
    /*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    if (L == NULL) // Ošetření ukazatele
        return;
    else if (DLActive(L) == 0) // Ošetření aktivity
        return;
    else
    {
        struct tDLElem *tmp = malloc(sizeof(struct tDLElem));
        if (tmp == NULL)
        {
            DLError();
            return;
        }
        tmp->data = val;

        tmp->rptr = L->Act;       // Zápis předka
        tmp->lptr = L->Act->lptr; // Zápis následníka

        L->Act->lptr->rptr = tmp; // Zápis nového předka do původního následníka Act
        L->Act->lptr = tmp;       // Zápis nového následníka do Act
    }
}

void DLCopy(tDLList *L, int *val)
{
    /*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
    if (L == NULL) // Ošetření ukazatele
        return;
    else if (DLActive(L) == 0) // Ošetření aktivity
    {
        DLError();
        return;
    }
    else
        *val = L->Act->data;
}

void DLActualize(tDLList *L, int val)
{
    /*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
    if (L == NULL) // Ošetření ukazatele
        return;
    else if (DLActive(L) == 0) // Ošetření aktivity
        return;
    else
        L->Act->data = val;
}

void DLSucc(tDLList *L)
{
    /*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
    if (L == NULL) // Ošetření ukazatele
        return;
    else if (DLActive(L) == 0) // Ošetření aktivity
        return;
    else if (L->Act == L->Last)
        L->Act = NULL;
    else
        L->Act = L->Act->rptr;
}

void DLPred(tDLList *L)
{
    /*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
    if (L == NULL) // Ošetření ukazatele
        return;
    else if (DLActive(L) == 0) // Ošetření aktivity
        return;
    else if (L->Act == L->First)
        L->Act = NULL;
    else
        L->Act = L->Act->lptr;
}

int DLActive(tDLList *L)
{
    /*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
    return L->Act == NULL ? FALSE : TRUE;
}

/* Konec c206.c*/
