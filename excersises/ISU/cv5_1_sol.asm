; const short values[] = { 5, 10, -2, -100, 256, 4, -101, 8 };
; 
; int main(int argc, char *argv[])
; {
;     short min = values[0];
; 
;     for(int i = 0; i < 8; ++i)
;     {
;         short y = values[i];
;         min = y ^ ((min ^ y) & -(min < y));
;     }
; 
;     WriteInt16(min);
;     WriteNewLine();
;     return 0;
; }

%include "rw32-2017.inc"

section .data
    values dw 5, -101, -2, -100, 256, 4, 10, 8

section .text
main:
    mov ebp, esp
    
    mov edi, 8
    
    mov dx, [values]
    
.loop:
    dec edi ; Posun indexu na predchozi hodnotu (o 1!!!)
    
    ; Naleznete minimum z hodnot v poli values (2B hodnoty se znamenkem).
    ; Pro nalezeni minima z dvojice hodnot pouzijte logicky vyraz:
    ; min = y ^ ((min ^ y) & -(min < y))
    ; Hodnotu min ulozte do registru DX, "y" ctete z pole values (values[EDI]).
    ; Aktualni index do pole je ulozen v registru EDI (od posledniho prvku).
    ;
    ; Cast vyrazu (min < y) je treba vyhodnotit za pomoci porovnani a registru FLAGS 
    ; a to tak, aby "(min < y) ? 1 : 0".
    ; Konkretne x < y => SF != OF (se znamenkem) nebo CF = 1 (bez znamenka)
    ; Pro cteni hodnot v registru FLAGS lze pouzit "LAHF" a "SETO reg".
    
    mov bx, word [values + edi*2] ; Nacteni 2B hodnoty "y" na indexu "EDI" z pole values
    cmp dx, bx          ; Porovnani hodnoty "y" a "min" (v DX), tedy "(min < y)". Nastavi FLAGS
    seto al             ; Nacteme hodnotu priznaku OF do AL (AL = 1 pokud OF==1)
    lahf                ; Nacteme bsah FLAGS (0-7 bit) do registru AH
    shr ah, 7           ; SF je 7. bit, posuneme AH o 7 bitu do prava, tak aby SF byl na 0. bitu
    xor al, ah          ; v AL chceme mit 1, pokud SF != OF, jinak 0 (tomu odpovida XOR)
    
    and ax, 0x0001      ; Vynulujeme AX krom 0. bitu (kde mame 1 nebo 0), tedy cast vyrazu (min < y)
    neg ax              ; Aritmeticka negace, v AX mame "-(min < y)"
    
    xor dx, bx          ; Cast vyrazu "(min ^ y)" v DX
    and dx, ax          ; Cast vyrazu "((min ^ y) & -(min < y))" v DX
    xor dx, bx          ; Vysledek "y ^ ((min ^ y) & -(min < y))" v DX 
    
    test edi, edi ; Test ukonceni smycky (edi != 0)
    jnz .loop ; Skok na ".loop", pokud (edi != 0)
    
    mov ax, dx
    call WriteInt16
    call WriteNewLine
    
    ret
