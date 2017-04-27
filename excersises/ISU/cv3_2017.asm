; int main() {
;     unsigned char data[] = {32, 64, 128, 100};
;     short data2[] = {-128, -232};
;     int data3[] = {-64,-100};
;     
;     unsigned short sum = (unsigned short)data[0];
;     sum += (unsigned short)data[1];
;     sum += (unsigned short)data[2];
;     sum += (unsigned short)data[3];
;     
;     WriteInt16(sum);
;     WriteNewLine();
;     
;     short sum2 = (short)sum;
;     sum2 += data2[0];
;     sum2 += data2[1];
;     
;     WriteInt16(sum2);
;     WriteNewLine();
;     
;     int sum3 = (int)sum2;
;     sum3 += data3[0];
;     sum3 += data3[1];
;     
;     WriteInt32(sum3);
;     WriteNewLine();
;     
;     return 0;
; }
    
%include "rw32-2017.inc"

section .data
    data  db 32,64,128,100 ; SUM = 324
    data2 dw -128,-232     ; SUM = -36
    data3 dd -64,-100      ; SUM = -200

section .text
main:
    mov ebp, esp
    
    xor eax, eax ; Nulovani registru EAX
    xor ebx, ebx ; Nulovani registru EBX
    
    ; KROK 0
    ; Sectete pole 8bit cisel BEZ ZNAMENKA (unsigned char), ktere zacina na "data".
    ; ** Vysledek ulozte v 16bit registru AX. **
    ; Potrebne instrukce: mov, add (pripadne xchg)
    
    mov al, [data]
    mov bl, [data+1]
    add ax, bx
    
    mov bl, [data+2]
    add ax, bx
    
    mov bl, [data+3]
    add ax, bx
    
    
    call WriteInt16     ; Vypis cisla v AX (se znamenkem)
    call WriteNewLine   ; Vypis ukonceni radku
    
    ; KROK 1
    ; K hodnote z predhoziho kroku prictete hodnoty z pole "data2", 
    ; ktere jsou 16bit SE ZNAMENKEM (signed short).
    ; ** Vysledek ulozte do 16bit registru AX. **
    
    mov bx, [data2]
    add ax, bx
    
    mov bx, [data2+2]
    add ax, bx
    
    
    call WriteInt16     ; Vypis cisla v AX (se znamenkem)
    call WriteNewLine   ; Vypis ukonceni radku
    
    ; KROK 2
    ; K vysledku z predhoziho kroku prictete hodnoty z pole "data3".
    ; Hodnoty jsou 32bit SE ZNAMENKEM (int).
    ; ** Vysledek ulozte na 32bitech v registru EAX. **
    ; Potrebne instrukce: mov, add, cwde
    ; "CWDE" prevede hodnotu *SE ZNAMENKEM* v registru AX na hodnotu v EAX taktez 
    ; *SE ZNAMENKEM* (tzv. signed extension AX -> EAX).
    
    cwde
    mov ebx, [data3]
    add eax, ebx
    
    mov ebx, [data3+4]
    add eax, ebx
    
    call WriteInt32     ; Vypis cisla v EAX (se znamenkem)
    call WriteNewLine   ; Vypis ukonceni radku

    ret
