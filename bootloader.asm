org 0x7c00
bits 16
start: jmp boot

msg db "Welcome to My Operating System!", 0ah, 0dh, 0h

boot: 
    cli
    cld
    hlt

times 510 - ($-$$) db 0
dw 0xAA55
