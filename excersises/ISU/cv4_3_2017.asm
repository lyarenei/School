; Vypoctete plochu nasledujiciho obrazce:
;     +-----+
;    /|     |
;   / |     A
;  /  |     |
; +-C-+--B--+
; Delky hran A, B a C jsou ulozeny v poli hodnot typu word bez znamenka.
; Vypocet provedte v 16-bit (word) hodnotach bez znamenka.

%include "rw32-2017.inc"

section .data
    strA db "A: ",0
    strB db "B: ",0
    strC db "C: ",0
    strQuadArea db "Obsah obdelnika: ",0
    strTriArea db "Obsah trojuhelnika: ",0
    strTotalArea db "Celkova plocha: ",0
    edges: resw 3 ; {A, B, C}

section .text
main:
    mov ebp, esp
    
    mov esi, strA
    call WriteString
    call ReadUInt16
    mov [edges], ax
    
    mov esi, strB
    call WriteString
    call ReadUInt16
    mov [edges+2], ax
    
    mov esi, strC
    call WriteString
    call ReadUInt16
    mov [edges+4], ax
    
    ; Vypoctete plochu obdelnika o stranach A, B (ulozenych v poli "edges")
    ; Pocitejte v 16bit a vysledek ulozte do registru AX
    mov ax, [edges]
    mov bx, [edges+2]
    mul bx
    mov cx, ax
    
    mov esi, strQuadArea
    call WriteString
    call WriteUInt16
    call WriteNewLine
    
    ; Vypoctete plochu trojuhelnika o zakladne C a vysce A (ulozenych v poli "edges")
    ; Pocitejte v 16bit a vysledek ulozte do registru AX
    
    ; Zaokrouhlete plochu trojuhelnika.
    mov ax, [edges+4]
    mul bx
    mov bx, 2
    div bx
    add ax, dx
    
    mov esi, strTriArea
    call WriteString
    call WriteUInt16
    call WriteNewLine
    
    ; Vypoctete celkovou plochu a vysledek ulozte do registru AX
    add ax, cx
    
    mov esi, strTotalArea
    call WriteString
    call WriteUInt16
    call WriteNewLine
    
    ret
