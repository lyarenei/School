%include "rw32-2017.inc"

%ifnmacro CEXTERN
%define CEXTERN extern
%endif

CEXTERN rand

RAND_MAX equ 0x7FFF
NUM_ITERS equ 1000000

section .data
    const3 dq 3.0
    const4 dq 4.0
    sphereRadius dq 2.5 ; Vysledek V = 65.449847
    strFPUStatus db "BCTTTCCCISPUOZDI",10," 3   210RF      ",0
    strSphereVolume db "Sphere volume = ",0
    strPIValue db "PI Value = ",

section .text
main:
    mov ebp, esp; for correct debugging
    push ebp
    mov ebp, esp
    
; Zde inicializuje jednotku FPU
; *** ZDE DOPLNTE VAS KOD ***
    finit
    
    call ShowFPUStatus
    
    sub esp, 8
    fld qword [sphereRadius]
    fstp qword [esp]
    call SphereVolume
    add esp, 8
    
    mov esi, strSphereVolume
    call WriteString
    call WriteDouble
    call WriteNewLine
    call WriteNewLine
    fstp st0
    
    call ShowFPUStatus
    
    push dword NUM_ITERS
    call ComputePI
    add esp, 4
    
    mov esi, strPIValue
    call WriteString
    call WriteDouble
    call WriteNewLine
    fstp st0
    
    call ShowFPUStatus
    
    mov esp, ebp
    pop ebp
    ret

; ** UKOL 1 **
; double SphereVolume(double r)
; {
;     return (4.0 / 3.0) * M_PI * r*r*r;
; }
SphereVolume:
    push ebp
    mov ebp, esp
    
; Doplnte telo funkce pro vypocet objemu koule dle: V = (4/3)*pi*r^3
; Funkce prijima jediny argument typu *DOUBLE* a vraci hodnotu objemu
; v registru *ST0* FPU (take *DOUBLE*).
; Pri vypoctu muzete pouzit konstanty definovane v datovem segmentu
; (4 - const4 a 3 - const3), obe jsou typu *DOUBLE*

; *** ZDE DOPLNTE VAS KOD ***

    fld qword [const4]              ; st0=4
    fld qword [const3]              ; st0=3
    fdivp                           ; st1=st1/st0 a pop => st0
    
    fldpi                           ; st0=pi st1=4/3
    
    fld qword [ebp+8]               ; st0=r st1=pi st2=4/3
    fld st0                         ; st0=r st1=r st2=pi st3=4/3
    fmul st0                        ; st0=r*r st1=r st2=pi st3=4/3
    fmulp                           ; st0=r^3 st1=pi st2=4/3
    
    fmulp                           ; st0=pir^3 st1=4/3
    fmulp                           ; st0=4/3 * pi * r^3
        
                
    mov esp, ebp
    pop ebp
    ret

; ** UKOL 2 **
; double ComputePI(unsigned int iterations)
; {
;     i = iterations;
;     int inside = 0;
;     
;     do
;     {
;         double x = GetRandomNumber();
;         double y = GetRandomNumber();
;         
;         double d = x*x + y*y;
;         
;         if(d <= 1)
;             inside++;
;     } while(i-- > 0);
;     
;     return 4.0 * ((double)inside / (double)iterations);
; }
ComputePI:
    push ebp
    mov ebp, esp

; Doplte telo funkce, ktera metodou montecarlo provadi vypocet approximace hodnoty PI.
; PI je mozne aproximovat jako pomer poctu nahodne zvolenych bodu v 2D plose (2x2), 
; ktere padnou do (+) a vne (-) kruznice o polomeru 1.
; Ulohu je mozne zjednodusit na vypocet PI/4 a uvazovanim pouze 1/4 kruznice a plochy.
; Pak lze zjistit vzdalenost bodu od stredu kruznice jako: d = x^2 + y^2 a plati, ze 
; pokud lezi uvnitr pak d <= 1.
;
; +-----------------+
; |***        x     |
; |   ***        x  |
; |       **        |
; |         **   x  |
; |    +      **    | 1
; |            *    |
; |  +          **  |
; |       +        *|
; |                *|
; +-----------------+
;          1
; Ukolem je tedy spocitat pocet bodu uvnitr kruznice a jejich pocet podelit 
; celkovym poctem bodu (poctem iteraci). Tim ziskame hodnotu PI/4, kterou 
; vynasobime 4mi a vyslednou hodnotu vratime v *ST0*. Tedy PI = 4*(I/N), kde I 
; je pocet bodu uvnitr kruznice.

; *** ZDE DOPLNTE VAS KOD **

    mov ecx, [ebp+8]                ; limit iteraci
    mov eax, 0                      ; inside
    push dword 1                    ; 1 pro porovnavani
    fild dword [esp]                ; st0=1
    
    do:
    push ecx                        ; zaloha iteraci
    push eax                        ; zaloha inside
    call GetRandomNumber            ; st0=x st1=1
    fld st0                         ; st0=x st1=x st2=1
    fmulp                           ; st0=x^2 st1=1    

    call GetRandomNumber            ; st0=y st1=x^2 st2=1
    pop eax                         ; obnoveni inside
    pop ecx                         ; obnoveni pocitani
    
    fld st0                         ; st0=y st1=y st2=x^2 st3=1
    fmulp                           ; st0=y^2 st1=x^2 st3=1
    
    fadd                            ; st0=y^2 + x^2 st1=1
    
    fcomip                          ; porovnani st0 a st1
    
    ja cykl
        inc eax                     ; inkrementace inside
    cykl:
        loop do
    
    finit
;     return 4.0 * ((double)inside / (double)iterations);
    push eax                        ; predani inside
    fild dword [esp]                ; st0=inside
    add esp, 4                      ; uklid
    
    fild dword [ebp+8]              ; st0=iterations st1=inside
    
    fdivp                           ; st1=inside/iterations pop <= st0=inside/iterations
    push dword 4
    fild dword [esp]                ; st0=4 st1=podil
    add esp, 4                      ; uklid
    
    fmulp                           ; st0= 4 * podil
    
    
    mov esp, ebp
    pop ebp
    ret

; Funkce vraci nahodne cislo z intervalu <0.0, 1.0> v registru *ST0* 
; (zbytek FPU registru bude posunut na ST1, ST2, atd.).
; Funkce zalohuje stav FPU, ale upravuje registry dle CDECL.
; double GetRandomNumber()
GetRandomNumber:
    push ebp
    mov ebp, esp
    sub esp, (108 + 8)                      ; Rezervace zasobniku pro stav FPU (108b) a 2x 4b promnenne
    
    fsave [ebp - 108]                       ; Ulozeni stavu FPU a inicializace
    
    call rand                               ; Volani C funkce pro generovani pseudo-nahodnych cisel
    and eax, RAND_MAX                       ; Omezeni maxima na RAND_MAX
    mov dword [ebp - (108 + 4)], eax        ; Ulozeni nahodne hodnoty na zasobnik
    mov dword [ebp - (108 + 8)], RAND_MAX   ; Ulozeni maxima nahodnych hodnot na zasobnik
    
    fild dword [ebp - (108 + 4)]            ; Cteni nah. hodnoty do FPU; ST0 = EAX & RAND_MAX
    fild dword [ebp - (108 + 8)]            ; ST0 = RAND_MAX; ST1 = EAX & RAND_MAX
    fdivp                                   ; ST0 = (EAX & RAND_MAX) / RAND_MAX
    fstp qword [ebp - (108 + 8)]            ; Ulozeni ST0 na zasobnik a FPU je prazdne
    
    frstor [ebp - 108]                      ; Obnoveni ulozeneho stavu FPU ze zasobniku
    fld qword [ebp - (108 + 8)]             ; Nacteni vysledku ze zasobniku zpet do ST0; ST1 = ... (puvodni stav)
    
    add esp, (108 + 8)                      ; Uvolneni zasobniku
    mov esp, ebp
    pop ebp
    ret

; ** UKOL BONUS **
; double GetRandomNumberRD()
GetRandomNumberRD:
; Implementujte funkci GetRandomNumber pomoci instrukce "rdrand" (Ivy Bridge+), 
; ktera pouziva HW specializovany pro generovani nahodnych cisel. 
; RDRAND vraci 32 nahodnych bitu do specifikovaneho registru. Tuto hodnotu je treba 
; omezit pouze na kladna cisla a nasledne normalizovat do rozsahu <0.0, 1.0> 
; a vratit v registru *ST0*.

; *** ZDE DOPLNTE VAS KOD **
    
    ret

; void ShowFPUStatus()
; Funkce precte aktualni stav FPU a vypise ho do konzole
ShowFPUStatus:
    push eax
    push esi
    
    mov esi, strFPUStatus
    call WriteString
    call WriteNewLine
    
    xor ax, ax
    fstsw ax
    call WriteBin16
    call WriteNewLine
    
    pop esi
    pop eax
    ret
