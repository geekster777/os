org 0x7c00
bits 16
start: jmp boot

; Basic boot process
boot: 
    cli ; Clear interrupts
    cld ; Clear the direction flag

    ; Set a buffer for the initial stack to sit above
    mov ax, 0x50 
    mov es, ax 
    xor bx, bx
    
    mov al, 2 ; Read from the floppy
    mov ch, 0 ; Track 0
    mov cl, 2 ; Second sector
    mov dl, 0 ; Drive number
    mov dh, 0 ; Head number
    
    mov ah, 0x02 ; Number of sectors to read
    int 0x13     ; BIOS read from floppy

    ; Move all the segment pointers
    mov ax, 0x50
    mov ds, ax
    mov ss, ax
    
    jmp 0x500 ; Jump and execute the sector

    hlt 

; Fill the excess space with zeros, and the boot signiture in the last 2 bytes
times 510 - ($-$$) db 0
dw 0xAA55
