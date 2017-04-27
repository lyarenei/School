%include "rw32-2017.inc"

; Konstanty pro funkci pohybu "move"
LEFT  equ 0xFF00 ; Pohyb vlevo
RIGHT equ 0x0100 ; Pohyb vpravo
UP    equ 0x00FF ; Pohyb nahoru
DOWN  equ 0x0001 ; Pohyb dolu

section .data
    width db 16     ; Sirka mapy
    height db 16    ; Vyska mapy
    pos_x db 1      ; Aktualni pozice v ose X
    pos_y db 1      ; Aktualni pozice v ose Y
    
    ; Data mapy
    map db "+--------------+"
        db "|          B   |"
        db "|              |"
        db "|              |"
        db "|              |"
        db "|              |"
        db "|              |"
        db "|              |"
        db "|              |"
        db "|              |"
        db "|              |"
        db "|              |"
        db "|              |"
        db "|              |"
        db "|           B  |"
        db "+--------------+"
        
section .bss
    dirString: resb 256 ; Misto pro ulozeni vstupniho retezce s kodem pohybu

section .text
main:
    mov ebp, esp; for correct debugging
    push ebp
    mov ebp, esp
    
    movzx eax, byte [width]
    movzx edx, byte [pos_y]
    mul edx
    movzx edx, byte [pos_x]
    add eax, edx
    mov byte [map + eax], '@'
    
    ; Vykresleni mapy na zacatku
    call drawMap
    
    ; UKOL 2: Nactete retezec se vstupnim kodem pro pohyb maximalni delce 256 
    ; a ulozte ho na adresu "dirString"
    ;
    ; *** ZDE DOPLNTE VAS KOD ***
    mov edi, dirString          ;adresa kam nacist
    mov ebx, 255                ;max pocet znaku
    call ReadString
    
    mov esi, dirString
.mainLoop:
    mov al, [esi]
    cmp al, 0
    je .exitMainLoop
    
    ; UKOL 3: Doplnte logiku zpracovani vstupniho retezce.
    ; Znaky retezce znamenaji smer pohybu:
    ; L - vlevo, R - vpravo, U - nahoru, D - dolu
    ; Pro vykonani pohybu volejte funkci "move" s parametrem v EAX
    ; Napr.: move(LEFT) pro pohyb vlevo
    ;
    ; switch(al) {
    ;     case 'L': move(LEFT);
    ;         break;
    ;     case 'R': move(RIGHT);
    ;         break;
    ;     case 'U': move(UP);
    ;         break;
    ;     case 'D': move(DOWN);
    ;         break;
    ;     default:
    ;         goto .exitMainLoop;
    ; }
    ;
    ; *** ZDE DOPLNTE VAS KOD ***
    
    cmp al, 'L'
    je .left
    
    cmp al, 'R'
    je .right
    
    cmp al, 'U'
    je .up
    
    cmp al, 'D'
    je .down
    
    jmp .exitMainLoop
    
    .left:
        mov eax, LEFT
        jmp .end
    
    .right:
        mov eax, RIGHT
        jmp .end
    .up:
        mov eax, UP
        jmp .end
        
    .down:
        mov eax, DOWN
        
    .end:
    
    call move
    
    inc esi
    jmp .mainLoop
.exitMainLoop:

    ; Vykresleni mapy po provedeni vsech kroku
    call drawMap

    mov esp, ebp
    pop ebp
    ret

; Funkce pro vykresleni mapy o velikosti width x height
; Neprijima zadne argumenty -- pristupujte ke globalnim promnennym
drawMap:
    pushad
    
    ; UKOL 1: Doplnte vykresleni 2D mapy v poli "map" pomoci funkci "WriteChar" 
    ; a "WriteNewLine".
    ;
    ; char *p = map;
    ; for(int y = 0; y < *height; ++y) {
    ;     for(int x = 0; x < *width; ++y) {
    ;         WriteChar(*p);
    ;         p++;
    ;     }
    ;     
    ;     WriteNewLine();
    ; }
    ;
    ; *** ZDE DOPLNTE VAS KOD ***
    xor ebx, ebx                ;y
    xor ecx, ecx                ;x
    mov esi, map                ;char *p
    cld                         ;smer lodsb
    
    
    for:
        inc ebx                 ;++y
        mov cl, 0               ;vynulovani x
        for_in:
              inc cl            ;++x
              lodsb             ;nacteni znaku do al a posunuti esi
              call WriteChar    ;...
              cmp cl, [width]   ;podminka cyklu
              jb for_in
        call WriteNewLine
        cmp bl, [height]        ;podminka cyklu
        jb for 
    
    popad
    ret

; Funkce pro provedeni pohybu z aktualni pozice ve smeru LEFT, RIGHT, UP, DOWN
; a jejich kombinaci (pomoci bitoveho OR).
; Argument urcujici smer pohybu je predan v registru EAX
move:
    pushad
    
    ; Zaloha argumentu funkce
    push eax
    
    ; Vypocet adresy z aktualni pozice
    movzx ecx, byte [pos_x]
    movzx edx, byte [pos_y]
    movzx eax, byte [width]
    imul eax, edx
    add eax, ecx

    lea esi, [map + eax]
    
    ; Nacteni argumentu
    pop eax
    
    ; Vypocet offsetu pro pohyb v pozadovanem smeru (DIR_Y * width + DIR_X)
    mov bl, ah      ; Extrakce DIR_X
    movsx ecx, bl   ; Znamenkove rozsireni DIR_X
    movsx edx, al   ; Znamenkove rozsireni DIR_Y
    movzx eax, byte [width] ; Nacteni sirky
    imul eax, edx   ; DIR_Y * width ...
    add eax, ecx    ; ... + DIR_X
    
    ; Smycka pro provedeni pohybu, dokud nenarazi na jiny znak nez ' '
    ; Cesta je naznacena znaky 'x', jeji konec pak znakem '@'
.moveLoop:
    mov bl, ' '
    cmp bl, [esi + eax]
    jne .moveLoopExit
    
    add esi, eax
    mov byte [esi], 'x'
    
    add [pos_x], ecx
    add [pos_y], edx

    jmp .moveLoop
.moveLoopExit:

    mov [esi], byte '@'
    
    popad
    ret
