%include "rw32-2017.inc"

section .data
    szShereVolume db "Volume is: ",0
    szPolynomialValues db "Polynomial values are: ",0
    szMapNotation db " => ",0
    szPolynomialRootAt db "Found polynomial root at: ",0
    
; Testovaci hodnoty pro ULOHA 1    
    radius dd 5
    angle45 dq 0.78539816339744830962 ; 45*
    angle10 dq 0.17453292519943295769 ; 10*
    
; Testovaci hodnoty a konstanty pro ULOHA 2
    const2 dq 2.0
    const3 dq 3.0
    const4 dq 4.0
    const6 dq 6.0
    values dd 3.0,3.0,2.0,1.0,0.0,-1.0,-2.5,-4.0
    
; Testovaci hodnoty pro ULOHA 3
    left dd -3.0    ; min
    right dd 1.0    ; max
    steps dd 20     ; pocet kroku

section .text
main:
    mov ebp, esp; for correct debugging
    push ebp
    mov ebp, esp
    
; Zde doplnte volani funkce pro vypocet objemu vysece koule: SphereSecVolume(int r, double phi)
; Pouzijte volaci konvenci *cdecl*
; Jako argumenty predejte *radius* a *angle45* (predani tzv. hodnotou)

; *** ZDE DOPLNTE VAS KOD ***
    push dword [angle10+4]
    push dword [angle10]
    
    push dword [radius]
    
    call SphereSecVolume
    add esp, 12
    
    mov esi, szShereVolume
    call WriteString
    call WriteDouble
    call WriteNewLine
    call WriteNewLine
    
    mov esi, szPolynomialValues
    call WriteString
    call WriteNewLine
    
    mov ecx, 7
.polynomialTestLoop:
    push ecx
    
    fld dword [values + 4*ecx]
    call WriteDouble
    
; Zde doplnte volani funkce pro vypocet hodnoty polynomialu: Polynomial(float x)
; Pouzijte volaci konvenci *cdecl*
; Jako argument predejte hodnotu typu *float* adresovanou pomoci *ECX* v poli *values*.

; *** ZDE DOPLNTE VAS KOD ***
    finit
    push dword [values + 4*ecx]
    call Polynomial
    add esp, 4
    
    mov esi, szMapNotation
    call WriteString
    
    call WriteDouble
    call WriteNewLine
    
    pop ecx
    loop .polynomialTestLoop
    
    call WriteNewLine
    
    mov esi, szPolynomialRootAt
    call WriteString
        
    push dword [steps]
    push dword [right]
    push dword [left]
    call SearchRoot
    add esp, 3*4
    
    call WriteDouble
    call WriteNewLine
    
    mov esp, ebp
    pop ebp
    ret
    
; ** ULOHA 1 - 3b**
;
; double SphereSecVolume(int r, double phi) {
;     return ((2.0f*M_PI*r*r*r) / 3.0f) * (1.0f - cosf(phi));
; }
;
; Priklad vysledku: 
; r=5 phi=45* => 76.6793
; r=5 phi=10* => 3.97732
SphereSecVolume:
    push ebp
    mov ebp, esp

; Doplnte telo funkce pro vypocet objemu vysece koule (viz. https://en.wikipedia.org/wiki/Spherical_sector),
; dle: V = ((2*pi*r^3) / 3) * (1 - cos(phi)).
; Funkce prijima 2 argumenty typu *int* a *double*
; - int r - polomer koule (4B cele cislo v doplnkovem kodu).
; - double phi - polovina uhlu vysece v radianech (8B cislo s plovouci radovou carkou).
; Vysledek vracejte v registru FPU *ST0*

; *** ZDE DOPLNTE VAS KOD ***
    ;druha zavorka
    fld qword [ebp+12]                      ;st0=phi
    fcos                                    ;st0=cos(phi)
    fld1                                    ;st0=1 st1=cos(phi)
    fsubrp                                  ;st1=st0-st1 a pop => st0
    
    ;prvni zavorka
    fild dword [ebp+8]                      ;st0=r st1=druha_zavorka
    fld st0                                 ;st0=r st1=r st2=druha_zavorka
    fld st0                                 ;st0=r st1=r st2=r st3=druha_zavorka
    fmulp                                   ;st0=r^2 st1=r st2=druha_zavorka
    fmulp                                   ;st0=r^3 st1=druha_zavorka
    fldpi                                   ;st0=pi st1=r^3 st2=druha_zavorka
    fmulp                                   ;st0=pi * r^3 st1=druha_zavorka
    fld1                                    ;st0=1
    fadd st0                                ;st0=2
    fmulp                                   ;st0=2 * pi * r^3 st1=druha_zavorka
    fld1                                    ;st0=1
    fadd st0                                ;st0=2
    fld1                                    ;st0=1 st1=2
    faddp st1                               ;st0=3
    fxch st1                                ;st0=2 * pi * r^3 st1=3
    fdivrp                                  ;st0=prvni_zavorka st1=druha_zavorka
    fmulp
    
    mov esp, ebp
    pop ebp
    ret

; ** ULOHA 2 - 2b **
;
; double Polynomial(float x) {
;     return (x*x*x + 3*x*x - 6*x) / 4 - 2;
; }
; 
; Priklad vysledku:
; x = -4.0 => 0.0
; x = -2.5 => 2.531250
; x = -1.0 => 0.0
; x = 0.0  => -2.0
; x = 1.0  => -2.5
; x = 2.0  => 0.0
; x = 3.0  => 7.0
Polynomial:
    push ebp
    mov ebp, esp
    
; Doplnte telo funkce pro vypocet hodnoty polynomu 3. radu daneho vzorcem:
; ((x^3 + 3*x^2 - 6*x) / 4) - 2
; Funkce prijima jediny argument typu *float*
; - float x (4B cislo s plovouci radovou carkou).
; Vysledek vracejte v registru FPU *ST0*
    
; *** ZDE DOPLNTE VAS KOD ***
    fld dword [ebp+8]
    fld st0
    fld st0
    fmulp
    fmulp                                   ;st0=x^3
    
    fld dword [ebp+8]
    fld st0
    fmulp                                   ;st0=x^2 st1=x^3
    fld qword [const3]
    fmulp                                   ;st0=3 * x^2 st1=x^3
    
    faddp                                   ;st0=x^3 + 3*x^2
    
    fld dword [ebp+8]
    fld qword [const6]
    fmulp                                   ;st0=6x st1=x^3 + 3*x^2
    fsubp                                  ;st0=prvni_zavorka
    
    fld qword [const4]
    fdivp                                  ;st0=druha_zavorka
    
    fld qword [const2]                  
    fsubp                                  ;st0=vysledek
    
    mov esp, ebp
    pop ebp
    ret

; ** ULOHA 3 - 2+3b **
;
; float SearchRoot(float min, float max, int steps)
; {
;     float step = (max - min) / steps;
;     float rootX = min;
;     float rootVal = fabs(Polynomial(rootX));
;    
;     float x = min;
;     for(int i = steps; i > 0; i--) {
;         float val = fabs(Polynomial(x));
;         if(val < rootVal) {
;             rootX = x;
;         }
;
;         x += step;
;     }
;     
;     return rootX;
; }
SearchRoot:
    push ebp
    mov ebp, esp
    
; Doplnte telo funkce SearchRoot(float min, float max, int steps), ktera
; rozdeli oblast od *min* po *max* na *steps* intervalu a v kazdem z nich 
; vypocita hodnotu funkce Polynomial(x) a vypise ji. - ** 2b **
; Dale upravte funkci, tak aby nasla pozici libovolneho korenu tohoto polynomu
; a vratila ji v registru FPU *ST0*. - ** 2 + 3b **
; Neni nutne resit presnost nalezeni korene (je dana poctem kroku a velikosti intervalu),
; ani ktery ze 3 korenu bude nalezen.
;
; Funkce ma 3 argumenty typu *float*, *float* a *int*
; - float min - spodni mez prohledavaneho intervalu (4B cislo typu float)
; - float max - horni hrani prohledavaneho intervalue (4B cislo typu float)
; - int steps - pocet kroku v prohledavanem intervalu (4B cislo typu int)

; *** ZDE DOPLNTE VAS KOD ***
    sub esp, 16
    finit
;   float step = (max - min) / steps;
    fld dword [ebp+12]                  ;max
    fld dword [ebp+8]                   ;min
    fsubp
    fild dword [ebp+16]
    fdivp
    fstp dword [esp+12]

;   float rootVal = fabs(Polynomial(rootX));
    push dword [ebp+8]
    call Polynomial
    add esp, 4
    fabs
    fstp dword [esp+8]


;   float step      @     esp+12
;   float rootVal   @     esp+8
;   float x         @     esp+4
;   float rootX     @     esp
;   float x = min   @     ebp+8

;   for(int i = steps; i > 0; i--) {
;       float val = fabs(Polynomial(x));
;       if(val < rootVal) {
;           rootX = x;
;       }
;         x += step;
;     }
    
    mov ecx, [ebp+16]                   ;int i
    
cykl:
    finit
    push dword [esp+4]                  ;x
    call Polynomial
    add esp, 4
    fabs                                ;st0=val
    fld dword [esp+8]                   ;st0=rootVal st1=val
       
    fcomip                              ;porovnani
    jae skip
    mov eax, [esp+4]
    mov [esp], eax                      ;rootX = x

    skip:
        mov eax, [esp+4]
        add eax, [esp+12]               ;x + step
        mov [esp+4], eax

    loop cykl
    
    
    end:
    mov eax, [esp]
    add esp, 16

    mov esp, ebp
    pop ebp
    ret
