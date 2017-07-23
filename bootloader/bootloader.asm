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
    
    mov al, 12 ; Number of sectors to read
    mov ch, 0 ; Track 0
    mov cl, 2 ; Second sector
    mov dl, 0 ; Drive number
    mov dh, 0 ; Head number
    
    mov ah, 0x02 ; Read from the floppy
    int 0x13     ; BIOS read from floppy

    ; Activate a20 line for all 4GB of memory
    mov ax, 0x2403 
    int 0x15 

    ; Move the GDT to 0x500
    mov si, gdt_start
    mov di, 0x500
    mov [gdt_ptr+2], di
    mov ecx, (gdt_end - gdt_start)
    mov ax, 0
    mov es, ax
    mov ds, ax
    rep movsb
    cld
    lgdt [gdt_ptr]

    ; Enter protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Long jump to protected mode code
    jmp 08h:pmode

    bits 32
pmode:
    ; Move the loaded code into kernel space
    mov esi,  0x1500
    mov edi,  0x100000
    mov ecx,  0xC200
    rep movsb    
    
    jmp 0x100000 ; Jump and execute the sector
    bits 16
    
    hlt 

; Global Descriptor Table
gdt_start:
    dw 0,0,0,0
code_seg:
    dw 0xffff ; 4 GB
    dw 0x0000
    db 0x00
    db 0x9A ; Can't edit
    db 0x4f ; Byte granularity & protected mode
    db 0x00
data_seg:
    dw 0xffff ; 4 GB
    dw 0x0000
    db 0x00
    db 0x92 ; Can edit
    db 0x4f ; Byte granularity & protected mode
    db 0x00
gdt_end:
gdt_ptr:
    dw (gdt_end-gdt_start-1) ; Size
    dd 0x0                   ; GDT ptr

; Fill the excess space with zeros, and the boot signiture in the last 2 bytes
times 510 - ($-$$) db 0
dw 0xAA55
