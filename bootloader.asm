org 0x7c00
bits 16
start: jmp boot

; Simple hello world message to be displayed in the future
msg db "Welcome to My Operating System!", 0ah, 0dh, 0h

; Basic boot process
boot: 
    cli ; Clear interrupts
    cld ; Clear the direction flag
    hlt 

; Fill the excess space with zeros, and the boot signiture in the last 2 bytes
times 510 - ($-$$) db 0
dw 0xAA55
