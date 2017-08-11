#include "terminal.h"
#include "stdio.h"
#include "asm_utils.h"
#include "interrupt.h"

static uint16_t entries_size;
static idt_entry* entries;

// This is the table of function pointers to be accessed in isr_stubs.asm
void ( *__isr_table[256] )( uint32_t vector, uint32_t err );

void set_idt_entry(uint16_t entry_num, void (*func)(void));

void default_handler(uint32_t vector, uint32_t err)
{
    if( vector >= 0x20 && vector < 0x30 )
    {
        __outb( 0x20, 0x20 );
        if(vector >= 0x28)
            __outb( 0xa0, 0x20 );
    }
    else
    {
        terminal_set_pos(3,0);
        terminal_print("Oops? ");
        terminal_print_byte((uint8_t)vector);

        // Tell the PIC to re-enable interrupts
        __outb(0x20, 0x20); 
    }
}

void error_handler(uint32_t vector, uint32_t err)
{
    terminal_set_pos(3,0);
    printf("Oops? Vec: %d Err: %d", vector, err);
}

// Initializes the PIC to allow interrupts
void init_pic()
{
    // Send the initialization command to both PIC's
    __outb(0x20, 0x11);
    __outb(0xa0, 0x11);

    // Set the offsets into the IDT for the PICs
    __outb(0x21, 0x20);
    __outb(0xa1, 0x28);

    // Attach the master and slave lines of the PICS to each other. 
    // PIC 1 is the master, and PIC 2 is the slave on line 2
    __outb(0x21, 4);
    __outb(0xa1, 2);

    // Set both PIC's to 8086 mode
    __outb(0x21, 1);
    __outb(0xa1, 1);

    // Reset the PIC masks to allow interrupts on all lines
    __outb(0x21, 0x0);
    __outb(0xa1, 0x0);
}

// Masks a supplied interrupt (0-15), preventing it from being triggered
void interrupt_set_mask(uint8_t irq_line)
{
    if(irq_line < 8)
    {
        __outb( 0x21, __inb(0x21) | ( 1 << irq_line ) );
    }
    else if(irq_line < 16)
    {
        __outb( 0xa1, __inb(0xa1) | ( 1 << ( irq_line - 8 ) ) );
    }
}

// Unmasks a supplied interrupt (0-15), allowing it to be triggered
void interrupt_clear_mask(uint8_t irq_line)
{
    if(irq_line < 8)
    {
        __outb( 0x21, __inb(0x21) & ~( 1 << irq_line ) );
    }
    else if(irq_line < 16)
    {
        __outb( 0xa1, __inb(0xa1) & ~( 1 << ( irq_line - 8 ) ) );
    }
}

// Initializes the interrupt system, setting up the IDT, default ISR's, and PIC
void interrupt_init(idt* idt_ptr)
{
    entries = (idt_entry*)(idt_ptr->ptr_lower | (idt_ptr->ptr_upper << 16));
    entries_size = idt_ptr->size / 8; // 8 bytes per entry

    extern void ( *__isr_stub_table[256] )( void );
    
    for(uint32_t i=0; i<entries_size; i++)
    {
        set_idt_entry(i, __isr_stub_table[i]);
        if(i >= 0x20 && i <= 0x40)
            interrupt_register(i, default_handler);
        else
            interrupt_register(i, error_handler);
    }

    init_pic();
}

// Registers the supplied function to be called when the interrupt is triggered
void interrupt_register(uint16_t entry_num, void (*func)(uint32_t, uint32_t))
{
    if(entry_num < entries_size)
    {
        __isr_table[entry_num] = func;
    }
}

// Sets an entry in the IDT, and points it to a function (likely an ISR stub)
void set_idt_entry(uint16_t entry_num, void (*func)(void))
{
    if(entry_num >= entries_size) return;
    
    idt_entry* entry = &entries[entry_num];
    entry->offset1 = (int)func & 0xffff;
    entry->segment = 0x08;
    entry->zero = 0;
    entry->type_attr = 0x8e;
    entry->offset2 = (int)func >> 16 & 0xffff;
}
