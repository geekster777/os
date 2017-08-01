org 0x7c00
bits 16
%define tracks_per_read 3
start: jmp boot

floppy_reads:
db 0, 0
kernel_destination:
dd 0
kernel_code:
dd 0

; Reads 48 sectors (3 tracks) from the floppy disk at once. Any more and we
; will start to overwrite our bootloader code (located ot 0x7c00)
; Param: cx Track and sector to read from (ch: track, cl: sector)
read_from_floppy:
    ; Set a buffer for the initial stack to sit above
    mov ax, 0x60 
    mov es, ax 
    xor bx, bx
    
    mov al, tracks_per_read*16 ; Read 48 sectors per read (3 tracks) 
    mov dl, 0  ; Drive number
    mov dh, 0  ; Head number
    
    mov ah, 0x02 ; Read from the floppy
    int 0x13     ; BIOS read from floppy

    ret

; Basic boot process
boot: 
    cli ; Clear interrupts
    cld ; Clear the direction flag

    ; Activate a20 line for all 4GB of memory
    mov ax, 0x2403 
    int 0x15 

    ; Move the GDT to the beginning of our free memory
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

load_os:
    ; Clear segments
    mov ax, 0
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Get the number of times we've already read from the floppy
    mov ch, [floppy_reads]
    mov ax, tracks_per_read
    mul ch 
    mov ch, al
    mov cl, 2
    call read_from_floppy

enter_pmode:
    ; Enter protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Long jump to protected mode code
    jmp 08h:pmode_code


    ; Move the loaded code into kernel space
    bits 32

pmode_code:
    ; Number of bytes to moved so far
    xor  ecx, ecx
    xor  ebx, ebx
    mov cl, [floppy_reads]
    cmp cl, 0
    jne move_kernel_code

; Read the elf header for important information
read_elf:
    mov eax, [0x63c]
    mov [kernel_destination], eax
    mov eax, [0x618]
    mov [kernel_code], eax
    mov ebx, [0x638] ; Segment start

move_kernel_code:
    mov eax, 0x2000*tracks_per_read ; Each track is 16 sectors * 512 bytes
    mul ecx
    mov ecx, eax ; Move the lower result into ecx (assumes the product is small)

    ; Load physical address for the kernel plus our calculated offset
    mov eax, [kernel_destination]
    add eax, ecx 
    
    ; Calculate proper offset into kernel space to place code
    add ebx,  0x600
    mov esi,  ebx
    mov edi,  eax

    mov ecx,  0x2000*tracks_per_read ; Number of bytes to move
    mov dx, 0
    mov ds, dx
    mov es, dx
    rep movsb 
    
    ; Check if we've read the floppy enough times
    mov cl,   [floppy_reads]
    cmp cl,   10     ; Stop reading after 10 loads (~200kb)
    je exit_bootloader
    add cl,   1      ; Increment floppy reads
    mov [floppy_reads], cl
    
exit_32_bit:
    jmp 0x18:exit_pmode

    bits 16
exit_pmode:
    mov eax, 0x20
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax
    mov ss, eax

    mov eax, cr0
    and eax, 0xfffffffe
    mov cr0, eax

    jmp 0:load_os

    bits 32
exit_bootloader: 
    ; Read the entry point and offset to main from the elf
    mov eax, [kernel_code]
    
    ; Jump and execute the sector
    jmp eax

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
code_16_seg:
    dw 0xffff
    dw 0x0000
    db 0x00
    db 0x9A
    db 0x0f
    db 0x00
data_16_seg:
    dw 0xffff
    dw 0x0000
    db 0x00
    db 0x92
    db 0x0f
    db 0x00
gdt_end:
gdt_ptr:
    dw (gdt_end-gdt_start-1) ; Size
    dd 0x0                   ; GDT ptr

; Fill the excess space with zeros, and the boot signiture in the last 2 bytes
times 510 - ($-$$) db 0
dw 0xAA55
