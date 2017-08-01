;
; Assembly utils to be called by C
;

bits 32

;
; Name:   __outb, __outw, __outl
; Args:   int - port number to write to
;         int - value value to write to the port
; Return: Note
; 
; Write value out to a port for port-based io
;  
global __outb, __outw, __outl
  
__outb:
    enter 0, 0
    mov eax, [esp+12]
    mov edx, [esp+8]
    out dx,  al
    leave
    ret

__outw:
    enter 0, 0
    mov eax, [esp+12]
    mov edx, [esp+8]
    out dx,  ax
    leave
    ret

__outl:
    enter 0, 0
    mov eax, [esp+12]
    mov edx, [esp+8]
    out dx,  eax
    leave
    ret

;
; Name:   __inb, __inw, __inl
; 
; Args:   int - The port to read from
; 
; Return: int - The value read from the port
; 
; Read value from a port for port-based io
; 
global __inb, __inw, __inl
  
__inb:
    enter 0, 0
    xor eax, eax
    mov edx, [esp+8]
    in  al, dx
    leave
    ret

__inw:
    enter 0, 0
    xor eax, eax
    mov edx, [esp+8]
    in  ax, dx
    leave
    ret

__inl:
    enter 0, 0
    xor eax, eax
    mov edx, [esp+8]
    in  eax, dx
    leave
    ret

;
; Name: __cli
; 
; Disables interrupts by running the cli command
; 
__cli:
    cli
    ret

;
; Name: __sti
; 
; Enables interrupts by running the sti command
; 
__sti:
    sti
    ret
