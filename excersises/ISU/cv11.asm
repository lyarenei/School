%include "rw32-2017.inc"

;
; windows extern hack - "_" verze jsou cdecl
;  macro pod windowsem vymeni standardni nazvy sa "_" nazvy a deklaruje existenci spravnych symbolu
;
%ifnmacro CEXTERN
%define CEXTERN extern
%endif

; funkce pro otevreni a uzavreni souboru
CEXTERN fopen
CEXTERN fclose
; funkce pro nacteni a spracovani dat
CEXTERN qsort
CEXTERN fscanf
; funkce pro dynamicky alokovanou pamet
CEXTERN malloc
CEXTERN free

; maximalni delka nacteneho radku ze souboru
%define ROW_LEN 80
%defstr ROW_LEN_STR ROW_LEN

; maximalni delka cesty k souboru
%define FILENAME_LEN 32

;
; datovy segment
;
section .data
rezim_souboru db "r",0
format_pocet  db "%u",0
format_radku  db "%u;%",ROW_LEN_STR,"[^",13,10,"]",0

; chybove hlasky
re_otevreni  db "Nepodarilo se otevrit soubor",EOL,0
re_precteni   db "Neplatny format souboru",EOL,0
re_alokace   db "Nepodarilo se alokovat pamet",EOL,0

section .bss
; misto pro nazev souboru, vcetne ukoncovaci nuly
nazev_souboru resb FILENAME_LEN+1

;
; kodovy segment
;
section .text
; Funkce main vyuziva knihovnu jazyka C k nacteni souboru, manipulaci s daty a jejich vypsani.
;   Po precteni prvniho radku, na kterem je pocet zbyvajicich radku je alokovana potrebna pamet.
;   Pote je cely obsah souboru nacten, pricemz vstup se sklada z bezznamenkoveho cisla a retezce.
;   Toto cislo udava pozici, na ktere by radek mal byt. Tzn. serazenim a vypsanim vysledku je
;   obdrzen vysledek.
;
; Priklad vstupniho souboru:
;   2
;   5;aa
;   3;bb
;
; Vystup pro dany vstup:
;   bb
;   aa
;
; Jak je mozne videt, vstupni radky jsou serazeny podle cisla na jejich zacatku, ktery slouzi jako
;   klic. Toto cislo neni soucasti vystupu. Delka retezce za oddelovacem ';' je limitovana makrem
;   ROW_LEN vyse.
main:
  ; vytvorime si misto pro lokalni promenne
  ; [ebp- 4] soubor      -- FILE* (fopen)
  ; [ebp- 8] pocet_radku -- unsigned int, 4B siroky
  ; [ebp-12] data        -- void* (malloc)
  push ebp
  mov ebp,esp
  sub esp,12

  ; nacteme nazev souboru pomoci ReadString v rw32
  mov ebx,FILENAME_LEN
  mov edi,nazev_souboru
  call ReadString

.otevri_soubor:
  ; otevreme soubor
  ; FILE *fopen(const char *path, const char *mode);
  push rezim_souboru
  push nazev_souboru
  call fopen
  add esp,8
  mov [ebp-4],eax      ; ulozeni vysledku
  ; kontrola navratove hodnoty a osetreni chyb
  test eax,eax
  jnz .nacti_pocet_radku
  ; chyba - nepodarilo se otevrit soubor
  mov esi, re_otevreni
  call WriteString
  jmp .konec

.nacti_pocet_radku:
  ; nacteme pocet radku, tato informace se nachazi na prvnim radku souboru
  ; int fscanf(FILE *stream, const char *format, ...);
  lea ebx,[ebp-8]
  push ebx             ; &pocet_radku
  push format_pocet    ; formatovaci retezec
  push dword [ebp-4]   ; soubor
  call fscanf
  add esp,12
  ; kontrola navratove hodnoty a osetreni chyb
  cmp eax,1            ; fscanf vraci pocet uspesne nactenych promennych
  je .spocti_potrebnou_pamet
  ; chyba - nepodarilo se nacist pocet radku v souboru
  mov esi, re_precteni
  call WriteString
  jmp .uzavri_soubor

.spocti_potrebnou_pamet:
  ; na kazdy radek potrebujeme 4 (cislo) + ROW_LEN (max. delka retezce) bajtu
  mov eax,4+ROW_LEN    ; velikost jednoho radku
  mul dword [ebp-8]    ; vynasobime poctem radku
                       ; vysledek v edx:eax, kontrola preteceni
  test edx,edx
  jz .alokuj
  ; chyba - pocet radku je prilis velky, nestaci nam 4GB pameti
  mov esi, re_precteni
  call WriteString
  jmp .uzavri_soubor

.alokuj:
  ; v registru eax mame ulozenou potrebnu velikost
  ; void *malloc(size_t size);
  push eax             ; alokovana pamet
  call malloc
  add esp,4
  mov [ebp-12],eax     ; ulozeni vysledku - ukazatel na ziskanou pamet
  ; kontrola navratove hodnoty a osetreni chyb
  test eax,eax
  jnz .nacti_radky
  ; chyba - nepovedlo se nam alokovat pamet
  mov esi, re_alokace
  call WriteString
  jmp .uzavri_soubor

.nacti_radky:
  ; ukol 1: dopiste kod pro nacteni radku zo souboru do alokovane pameti
  ;
  ; [ebp-4]      - FILE* soubor
  ; [ebp-8]      - pocet radku (a iteraci cyklu)
  ; [ebp-12]     - ukazatel na alokovanou pamet
  ; format_radku - formatovaci retezec
  ;
  ; Z kazdeho radku souboru je nacteno 4B cislo bez znamenka (unsigned int) a retezec, ktereho
  ;   delka je maximalne ROW_LEN. Ulozte tyto data do pameti jako pole struktur, tzn.
  ;   <cislo_1><retezec_1><cislo_2><retezec_2><cis...
  ; V jazyce C by deklarace prvku tehle struktury vypadala nasledovne:
  ;   struct elem { unsigned int cislo; char retezec[ROW_LEN]; };
  ; K nacteni dat pouzijte funkci fscanf z knihovny jazyka c. Pokuste se osetrit pripadne chyby
  ;   pri nacteni (kontrolujte navratovu hodnotu fscanf, ktera by mela byt 2). Muzete se inspirovat
  ;   zbytkem kodu v tomhle souboru.
  ;
  ; zde doplnte vas kod
  xor ecx, ecx
  
  .cykl:
  mov eax, 4+ROW_LEN   ; delka jedne polozky
  mul ecx              ; nasobeni poctem cyklu
  add eax, [ebp-12]    ; ziskani konecne adresy
  push eax             ; predani konecne adresy pro zapis
  push format_radku    ; formatovaci retezec
  push dword [ebp-4]   ; nazev souboru
  call fscanf
  inc ecx
  add esp,12
  ; kontrola navratove hodnoty a osetreni chyb
  ; cmp eax,2            ; fscanf vraci pocet uspesne nactenych promennych
  inc ecx
  cmp dword ecx, [ebp-8]    ; podminka cyklu
  jl .cykl
  jmp .vypis

.serad:
  ; seradime posloupnost dle cisla pred retezcem
  ; void qsort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));
  push compare_rows    ; ukazatel na funkci pro porovnani
  push dword 4+ROW_LEN ; pocet bajtu na element (radek)
  push dword [ebp-8]   ; pocet elementu (radku)
  push dword [ebp-12]  ; ukazatel na data (pole)
  call qsort
  add esp,16

.vypis:
  ; v cykle vypiseme vsechny serazene radky
  mov ecx,[ebp-8]      ; ecx = pocet radku
  mov esi,[ebp-12]
  add esi,4            ; esi = adresa retezce

.vypis_cyklus:
  call WriteString
  call WriteNewLine
  add esi,4+ROW_LEN
  loop .vypis_cyklus

.dealokuj:
  ; void free(void *ptr);
  push dword [ebp-12]
  call free
  add esp,4

.uzavri_soubor:
  ; int fclose(FILE *stream);
  push dword [ebp-4]
  call fclose
  add esp,4

  ; konec
.konec:
  xor eax,eax
  leave
  ret

; funkce pro porovnani dvou elementu
; po vytvoreni zasobnikoveho ramce:
;   [ebp+ 8] prvni parametr - UKAZATEL na prvni prvek
;   [ebp+12] druhy parametr - UKAZATEL na druhy prvek
; vraci negativni cislo jestlize je prvni parametr mensi, 0 jestli se rovnaji
;   a pozitivni cislo jestlize je prvni parametr vetsi
; poznamka: nezapomente, ze vsechny registry krome eax,ecx a edx musite zalohovat (cdecl)!
compare_rows:
  ; ukol 2: doplnte telo funkce compare_rows, ktera je pouzita pro serazeni radku v pameti
  ;
  ; Jednotlive radky se radi podle bezznamenkoveho cisla umisteneho na zacatku elementu.
  ;   Muzete teda vstupni parametry povazovat za ukazatele na 4B cele cisla bez znamenka.
  ;
  ; zde doplnte vas kod
  
  push esp
  mov ebp, esp
  
  xor eax, eax
  xor ebx, ebx
  
  mov eax, [ebp+8]
  mov eax, [eax]        ; pristoupeni k prvku na ktery ukazuje adresa v eax
  mov ebx, [ebp+12]
  mov ebx, [ebx]        ; pristoupeni k prvku na ktery ukazuje adresa v ebx
  
  cmp eax, ebx
  jz equal
  cmp eax, ebx
  jg greater
  mov eax, -1
  jmp end
  
  greater:
  mov eax, 1
  jmp end
  
  equal:
  mov eax, 0
  
  end:
  mov esp, ebp
  pop esp

  ret
