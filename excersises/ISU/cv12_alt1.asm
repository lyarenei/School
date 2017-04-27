%include "rw32-2017.inc"

section .data

f_X dd 2.0
f_Y dd 3.0
f_Z dd 4.0

d_X dq 2.0
d_Y dq 3.0
d_Z dq 4.0

deset dq 10.0

polomer dd 15
ctyri dq 4.0
tri   dq 3.0

a dd 5
b dd 10

c       dq 299792458.0 ;rychlost svetla
half_c  dq 149896229.0 ;polovicni rychlost svetla
almost_c dq 296794533.42 ;99% rychlosti svetla

section .text

main:
    mov ebp, esp; for correct debugging

  finit

  ; zde volejte funkci rovnice, nezapomente uklidit zasobnik
  ; float rovnice(float x, float y, float z);
  
  push dword [d_Z+4]
  push dword [d_Z]
  
  push dword [d_Y+4]
  push dword [d_Y]
  
  push dword [d_X+4]
  push dword [d_X]
  call rovnice
  add esp, 24  

  call WriteDouble ;vypise vysledek ze st0
  call WriteNewLine

  finit

  ; zde volejte funkci objemKoule, nezapomente uklidit zasobnik
  ; double objemKoule(int polomer);
  
  push dword [polomer]
  call objemKoule
  add esp, 4
  
  call WriteDouble ;vypise vysledek ze st0
  call WriteNewLine

  finit

  ; zde zavolejte funkci Einstein
  ; double Einstein(double m0, double v)

  push dword [half_c+4]
  push dword [half_c]
  
  push dword [deset+4]
  push dword [deset]
  
  call Einstein
  add esp, 16
    
  call WriteDouble ;vypise vysledek ze st0
  call WriteNewLine 


  finit

  ; zde zavolejte funkci jeVetsi
  ; double jeVetsi(double x, double y)
  push dword [d_Y+4]
  push dword [d_Y]
  
  push dword [d_X+4]
  push dword [d_X]
  call jeVetsi
  add esp, 16

  call WriteDouble ;vypise vysledek ze st0
  call WriteNewLine


  ret ;konec main

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;; Implementujte funkci pro vypocet jednoduche rovnice
;;; Rovnice: x + y * z
;;; Jakmile budete mit funkcni verzi s float, zkuste verzi s double
;
;   ukazkovy kod v C
;
;   float rovnice(float x, float y, float z)
;   {
;     return x + y * z;
;   }
;

rovnice:
  push ebp
  mov ebp, esp
  
  fld qword [ebp+24]        ; st0=z
  fld qword [ebp+16]        ; st0=y st1=z
  fmulp                     ; st0=y*z
  
  fld qword [ebp+8]         ; st0=x st1=y*z
  faddp                     ; st0=vysledek
  
  mov esp, ebp
  pop ebp

  ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;; Implementujte funkci pro vypocet objemu koule
;;; V = 4/3 * PI * r^3
;;; Testy: polomer 5  => objem 523.599
;;;        polomer 10 => objem 4188.79
;;;        polomer 15 => objem 14137.166941
;
;   ukazkovy kod v C
;
;   double objemKoule(int polomer)
;   {
;     return 4.0/3.0*PI*polomer*polomer*polomer;
;   }
;


objemKoule:
    
    push ebp
    mov ebp, esp
    
    fld qword [ctyri]               ; st0=4
    fld qword [tri]                 ; st0=3 st1=4
    fdivp                           ; st1=4/3 a pop => st0=4/3
    
    fldpi                           ; st0=pi st1=4/3
    
    fild dword [ebp+8]              ; st0=r st1=pi st2=4/3
    fld st0                         ; st0=r st1=r st2=pi st3=4/3
    fmul st0                        ; st0=r*r st1=r st2=pi st3=4/3
    fmulp                           ; st0=r^3 st1=pi st2=4/3
    
    fmulp                           ; st0=pir^3 st1=4/3
    fmulp                           ; st0=4/3 * pi * r^3
    
    mov esp, ebp
    pop ebp
    ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;; Podle zakonu Specialni teorie relativity spocitejte relativni hmotnost
;;; pohybujiciho se telesa.
;;;
;;; Rovnice: m = m0 / (sqrt(1 - v^2/c^2))
;;; m0 - hmotnost telesa v klidu
;;; m  - relativni hotnost telesa v pohybu
;;; v  - rychlost pohybu telesa
;;; c  - rychlost svetla (priblizne 299 792 458 m/s)
;;;
;;; Testy: m0 = 10.0, v = c/10  => m = 10.050378
;;;        m0 = 10.0, v = c/2   => m = 11.547005
;;;        m0 = 10.0, v = 0.99c => m = 70.888121
;
; Kod v C:
; double Einstein(double m0, double v)
; {
;   return m0 / sqrt(1 - (v*v)/(c*c));
; }
;
; - m0 a v predavejte funkci jako parametry pres zasobnik, c muzete pouzit primo
; - nemusite kontrolovat, zda vstupni rychlost teorii porusuje


Einstein:

  ; zde napiste svuj kod, funkci zavolejte v mainu
  push ebp
  mov ebp, esp
  
  fld1                              ; st0=1 
  fld qword [c]                     ; st0=c st1=1
  fld qword [c]                     ; st0=c st1=c st2=1
  fmulp                             ; st0=c^2 st1=1
  
  fld qword [ebp+16]                ; st0=c/2 st1=c^2 st2=1
  fld st0                           ; st0=c/2 st1=c/2 st3=c^2 st4=1
  fmulp                             ; st0=(c/2)^2 st1=c^2 st2=1

  fdivrp                            ; st0=podil_rychlosti st1=1
  
  fsubp                             ; st1=st1-st0 a pop => st0=st1-st0
  fsqrt                             ; st0=sqrt(st0)
  
  fld qword [ebp+8]                 ; st0=m0 st1=sqrt..
  fdivrp                            ; st0=st0/st1 
  
  
  mov esp, ebp
  pop ebp
  ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;; Implementujte funkci pro zjisteni vetsiho cisla
;
;   ukazkovy kod
;
;   double jeVetsi(double x, double y)
;   {
;     if (x>y) return 1.0;
;     else     return 0.0;
;   }
;

jeVetsi:
  push ebp
  mov ebp, esp
  
  fld qword [ebp+16]
  fld qword [ebp+8]
  
  fcomip
  
  ja vetsix
  fldz
  jmp end
  
  vetsix:
  fld1
  
  end:

  mov esp, ebp
  pop ebp
  ret
