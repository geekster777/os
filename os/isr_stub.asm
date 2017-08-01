; 
; Declare a table of ISR dummy functions to call my C functions later
;

; This is the table of interrupts in C declared in interrupt.h
extern __isr_table 

; Declare the 256 ISR stubs using the macros
%assign i 0
%rep 256
ISR_%+i:
    pushad
    cld
    mov ax, i
    push ax
    call [__isr_table + i*4] ; 32 bit addresses
    pop  ax
    popad
    iret

%assign i i+1
%endrep

; A set of stubs to be loaded into the IDT in interrupt.h
global __isr_stub_table
__isr_stub_table:
%assign i 0
%rep 256
dd ISR_%+i
%assign i i+1
%endrep

