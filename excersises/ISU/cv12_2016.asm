%include "rw32-2017.inc"

section .data
    szInside db "Inside",0
    szOutside db "Outside",0
    szVolume db "Sphere volume is: ",0
    
    const4 dq 4.0
    const3 dq 3.0
    
    ; Test 1: Volume - 65.449847, Outside
    sphereCenter dd 0.0, 0.0, 0.0
    sphereRadius dq 2.5
    pointLocation dd 2, 2, 2
    
    ; Test 2: Volume - 179.594380, Inside
    ;sphereCenter dd 2.0, 3.0, 4.0
    ;sphereRadius dq 3.5
    ;pointLocation dd 4, 5, 6

section .text
main:
    mov ebp, esp; for correct debugging
    push ebp
    mov ebp, esp
    
; Zde volejte funkci SphereVolume pro vypocet objemu koule
; Jako argument ji predejte hodnotu promnenne typu double na adrese
; "sphereRadius".
; Pouzijte volaci konvenci "cdecl"

; *** ZDE DOPLNTE VAS KOD ***
    
    push dword [sphereRadius+4]
    push dword [sphereRadius]
    ;push qword [sphereRadius]    ; 64b verze
    
    call SphereVolume
    add esp, 8
    
    mov esi, szVolume
    call WriteString
    call WriteDouble
    call WriteNewLine
    
; Zde volejte funkci IsInSphere s argumenty s hodnotami na adresach: 
; pointLocation (3x 4B int), sphereCenter (3x float) 
; a sphereRadius (1x double).
; Pouzijte volaci konvenci "cdecl"

; int IsInSphere(int pointX, int pointY, int pointZ, 
;     float centerX, float centerY, float centerZ, double radius)


; *** ZDE DOPLNTE VAS KOD ***
    
    push dword [sphereRadius+4]
    push dword [sphereRadius]       
    ;push qword [sphereRadius]         ;64b verze
    
    push dword [sphereCenter+8]       ; centerZ
    push dword [sphereCenter+4]       ; centerY
    push dword [sphereCenter]         ; centerX
    
    push dword [pointLocation+8]      ; pointZ
    push dword [pointLocation+4]      ; pointY
    push dword [pointLocation]        ; pointX
    
    call IsInSphere
    add esp, 32                 ; 6*4 +8 uklid zasobniku
    
    mov esi, szInside
    mov ebx, szOutside
    cmp eax, 0
    cmove esi, ebx
    call WriteString
    call WriteNewLine
    
    mov esp, ebp
    pop ebp
    ret

; double SphereVolume(double r)
; {
;     return (4.0 / 3.0) * M_PI * r*r*r;
; }
SphereVolume:
    push ebp
    mov ebp, esp
    
; Doplnte telo funkce pro vypocet objemu koule dle: V = (4/3)*pi*r^3
; Funkce prijima jedniny argument typu *double* a vraci hodnotu obemu 
; v registru "ST0" FPU (take *double*).
; Pri vypoctu muzete pouzit konstanty nadefinovane v datovem segmentu
; (4 - const4 a 3 - const3), obe jsou typu *double*

; *** ZDE DOPLNTE VAS KOD ***
    
    fld qword [const4]              ; st0=4
    fld qword [const3]              ; st0=3
    fdivp                           ; st1=4/3 a pop => st0=4/3
    
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

; int IsInSphere(int pointX, int pointY, int pointZ, 
;     float centerX, float centerY, float centerZ, double radius)
; {
;     double diffX = pointX - centerX;
;     double diffY = pointY - centerY;
;     double diffZ = pointZ - centerZ;
;     
;     double dist = sqrt(diffX*diffX + diffY*diffY + diffZ*diffZ);
;     
;     if(dist < radius)
;         return 1;
;     else
;         return 0;
; }
IsInSphere:
    push ebp
    mov ebp, esp
    
; Doplnte telo funkce pro zjisteni, zda bod lezi uvnitr koule dle vzorce:
; SQRT((x - x0)^2 + (y - y0)^2 + (z - z0)^2) < r, 
; kde x,y,z je pozice bodu, x0,y0,z0 je pozice stredu koule a
; r je polomer koule.
; Argumenty funkce jsou x,y,z (3x 4B integer), x0,y0,z0 (3x float) a 
; r (1x double) - predavane na zasobniku (dle "cdecl" konvence)
; Navratovou hodnotu (0 - bod mimo kouli, 1 - bod uvnitr) vracejte 
; v registru EAX.

; *** ZDE DOPLNTE VAS KOD ***
    
    fld qword [ebp+32]       ; st0=r
    
    fld dword [ebp+20]      ;st0=x0 st1=r
    fild dword [ebp+8]       ;st0=x st1=x0 st2=r
    fsubp                   ;st0=x-x0 st1=r
    fld st0                 ;st0=x-x0 st1=x-x0 st2=r
    fmulp                   ;st0=x^2 st1=r
    
    fld dword [ebp+24]      ;st0=y0 st1=x^2 st2=r
    fild dword [ebp+12]      ;st0=y st1=y0 st2=x^2 st3=r
    fsubp st1               ;st0=y-y0 st1=x^2 st2=r
    fld st0                 ;st0=y-y0 st1=y-y0 st2=x^2 st3=r
    fmulp                   ;st0=y^2 st1=x^2 st2=r    

    fld dword [ebp+28]      ;st0=z0 st1=y^2 st2=x^2 st3=r
    fild dword [ebp+16]      ;st0=z st1=z0 st2=y^2 st3=x^2 st4=r
    fsubp                   ;st0=z-z0 st1=y^2 st2=x^2 st3=r
    fld  st0                ;st0=z-z0 st1=z-z0 st2=y^2 st3=x^2 st4=r
    fmulp                   ;st0=z^2 st1=y^2 st2^x^2 st3=r
    
    
    faddp                   ;st0=z^2 + y^2 st1=x^2 st3=r
    faddp                   ;st0=x^2 + y^2 + z^2 st1=r
    fsqrt                   ;st0=sqrt(st0) st1=r
    
    fcomip
    
    jb mensi_r
        mov eax, 0
    jmp end
    
    mensi_r:
    mov eax, 1
    
    end:
    mov esp, ebp
    pop ebp
    ret
    
