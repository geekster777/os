bits 32

section .bss
; Reserve space for the stack
align 4
stack_bottom:
    resb 16384
stack_top:

section .text
global _start:function (_start.end - _start)
_start:
    cli

    ; Initialize the segment registers    
    mov eax, 0x10
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax
    mov ss, eax
    
    ; Initialize the stack
    mov esp, stack_top
    
    ; Call the main C kernel code
    extern kernel_main
    call kernel_main
    cli

.hang:
    hlt
    jmp .hang
.end:
