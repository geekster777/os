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
    
    ; Initialize the interrupt data table
    lidt [idt_ptr]
    mov eax, idt_ptr
    push eax
    extern interrupt_init
    call interrupt_init

    extern keyboard_init
    sti
    
    ; Call the main C kernel code
    extern kernel_main
    call kernel_main
    cli

.hang:
    hlt
    jmp .hang
.end:

idt_ptr:
    dw 0x800  ; 256 interrupts
    dd 0x1000 ; Interrupt table location
