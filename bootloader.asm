org 0x7c00
bits 16
start: jmp boot

; Simple hello world message to be displayed in the future
msg db "Welcome to My Operating System!", 0ah, 0dh, 0h

; Basic boot process
boot: 
    cli ; Clear interrupts
    cld ; Clear the direction flag

    call ClearScreen

    mov bh, 0x1
    mov bl, 0x2
    call MoveCursor
    
    mov bh, 0x0
    mov bl, 0x0
    mov si, msg
    call Print     

    hlt 

; Clear the screen by scrolling it up
ClearScreen:
    mov al, 0x0
    mov bh, 0x0
    mov cx, 0x0
    mov dx, 0xffff
    mov ah, 0x6
    int 0x10
    ret

; Move the cursor to the x,y positions provided in bl,bh
MoveCursor:
    mov ah, 0x2 ; Interrupt number
    mov dh, bh  ; Set row
    mov dl, bl  ; Set column
    mov bh, 0x0 ; Page 0
    int 0x10
    ret

; Place a character on the screen in a selected color a number of times
PutChar:
    mov ah, 0x9 ; Interrupt number
    mov bh, 0x0 ; Page 0
    int 0x10
    ret

; Print a string
Print:
    mov  al, 0x0 ; Update cursor after writing
    mov  bh, 0x0 ; Page 0
    mov  bl, 0xb ; Color (cyan)
    mov  ecx, 0x0 ; Initial character count
    mov  bp, si  ; String pointer

; Count the characters in the string
_count_chars:
    mov dl, byte[ebp+ecx]
    cmp dl, 0x0
    je _print_str
    inc cx
    jmp _count_chars

; Restore the row, and call the print interrupt
_print_str:
    mov dx, 0x0303  ; Coordinates
    mov ah, 0x13 ; Interrupt number
    push ds
    pop  es      ; String start segment  
    int 0x10
    ret

; Fill the excess space with zeros, and the boot signiture in the last 2 bytes
times 510 - ($-$$) db 0
dw 0xAA55
