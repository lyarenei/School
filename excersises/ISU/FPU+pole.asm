%include "rw32-2017.inc"

section .data
    ; write your data here
    cisla resq 10
    vystup_text db "Nejmensi cislo je:",0
    vstup_text db "Zadejte 10 cisel:",0

section .text
main:
    mov ebp, esp
    
    ; write your code here   
    mov ecx, 10                             ;velikost pole
    mov eax, 0                              ;pocitadlo
    
    mov esi, vstup_text
    call WriteString
    call WriteNewLine
nacti:
    call ReadDouble                         ;st0=cislo
    fstp qword [cisla+(8*eax)]              ;ulozeni cisla do pole a pop z FPU
    inc eax                                 ;posun v poli
    loop nacti
    
    
    finit
    mov ecx, 10                             ;velikost pole
    mov eax, 1                              ;pocitadlo
    fld qword [cisla]                       ;st0=cislo1
    
cykl:
    
    fld qword [cisla+(8*eax)]               ;st0=cislo2 st1=cislo1
    inc eax                                 ;posun v poli
    
    fcomi                                   ;porovnej a nahraj flagy do CPU
    jb vyhod                                ;pokud true => st0=mensi cislo st1=vetsi cislo
    fstp st0                                ;st0=mensi cislo
    jmp repeat
vyhod:
    fxch st1                                ;st0=vetsi cislo st1=mensi cislo
    fstp st0                                ;zahozeni vetsiho cisla st0=mensi cislo
repeat:
    loop cykl
    
    mov esi, vystup_text
    call WriteString
    call WriteNewLine
    call WriteDouble
    
    xor eax, eax
    ret
