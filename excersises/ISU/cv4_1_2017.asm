; Vytvorte implementaci programu, ktery podeli dve cisla SE ZNAMENKEM
; a vysledek vypise ve tvaru:
; Podil: [CISLO]
; Zbytek: [CISLO]
; Vstupni cisla definujte v datovem segmentu (kazde na 4B - dword)
; Retezce: "Podil: " a "Zbytek: " definujte taktez v dat. segmentu jako pole 
; hodnot typu byte !!!(nezapomente ukoncovaci 0)!!!
;
; int main() 
; {
;     const char *pStrA = "Podil: ";
;     const char *pStrB = "Zbytek: ";
;     int a = 10;
;     int b = 3;
;     
;     int c = a / b;
;     int d = a % b;
;     
;     WriteString(pStrA);
;     WriteInt32(c);
;     WriteNewLine();
;     
;     WriteString(pStrB);
;     WriteInt32(d);
;     WriteNewLine();
;     
;     return 0;
; }

; *** ZDE DOPLNTE VAS KOD ***
%include "rw32-2017.inc"

section .data

    input1  dd 19
    input2  dd 5
    
    podil   db "PODIL:",0
    zbytek  db "ZBYTEK:",0

section .text

main:
    mov eax, [input1]
    div dword [input2]
    cwde
    
    mov esi, podil
    call WriteString
    call WriteInt32
    call WriteNewLine
    
    mov esi, zbytek
    call WriteString
    mov eax, edx
    call WriteInt32
    call WriteNewLine
    
    ret
