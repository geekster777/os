; 
; Declare a table of ISR dummy functions to call my C functions later
;

; This is the table of interrupts in C declared in interrupt.h
extern __isr_table 
extern scheduler_current_context

; ISR macro
%macro ISR 1
__isr_%+ %1:
    push dword 0 ; Error code placeholder
    push dword %1
    pushad
    jmp ISR_save

%endmacro

; Error ISR macro (they push down an extra error code)
%macro ISR_ERR 1
__isr_%+ %1:
    push dword %1
    pushad
    jmp ISR_save

%endmacro

; Declare the 256 ISR stubs using the macros

; First the error handlers
%assign i 0
%rep 8
ISR i
%assign i i+1
%endrep

ISR_ERR 8
ISR 9
ISR_ERR 10
ISR_ERR 11
ISR_ERR 12
ISR_ERR 13
ISR_ERR 14
ISR 15
ISR 16
ISR_ERR 17

; Next the ISR's
%assign i 18
%rep 238
ISR i
%assign i i+1
%endrep

ISR_save:
    
    ; Stack contents
    ; EDI ESI EBP ESP EBX EDX ECX EAX vec err EIP CS  EFL
    ; 0   4   8   12  16  20  24  28  32  36  40  44  48
    mov [scheduler_current_context], esp
    cld
    mov eax, [esp + 32]
    mov ebx, __isr_table
    call [eax*4 + ebx] ; 32 bit addresses

ISR_restore:
    mov esp, [scheduler_current_context]

    ; Restore the context
    popad
    
    ; Pop the vector and error number
    pop eax
    pop eax
    iret

; A set of stubs to be loaded into the IDT in interrupt.h
global __isr_stub_table
__isr_stub_table:
%assign i 0
%rep 256
dd __isr_%+i
%assign i i+1
%endrep

