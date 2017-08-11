/**
  * Interface to the Interrupt Descriptor Table.  
  */

#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include<stdint.h>

typedef struct idt_entry 
{
    uint16_t offset1;   // First half of the pointer to the ISR
    uint16_t segment;   // The GDT segment to use
    uint8_t  zero;      // Should be set to zero always
    uint8_t  type_attr; // The attributes of this entry. Likely 0Eh or 8Eh
    uint16_t offset2;   // Second half of the pointer to the ISR
} idt_entry;

typedef struct idt 
{
    uint16_t size;    // Number of ISR's in the table * 8 bytes per entry
    uint16_t ptr_lower;    // Pointer to the first entry of the table
    uint16_t ptr_upper;
} idt;

typedef struct interrupt_frame
{
    uint16_t ip;
    uint16_t cs;
    uint16_t flags;
    uint16_t sp;
    uint16_t ss;
} interrupt_frame;

/**
  * Name:   interrupt_init
  * Args:   idt* - Pointer to the Interrupt Descriptor Table
  * Return: None
  * 
  * Initializes the IDT and sets up the ISR's
  */
void interrupt_init(idt* idt_ptr); 

/**
  * Name:   interrupt_register
  * Args:   int   - The interrupt number
  *         void* - Pointer to the function to run upon the interrupt
  * Return: None
  *
  * Registers a function to be called everytime the interrupt is triggered
  */
void interrupt_register(uint16_t entry_num, void (*func)(uint32_t, uint32_t));

/**
  * Name:   interrupt_set_mask
  * Args:   int - The interrupt to be masked out (0-15 referring to PIC ISR's)
  *
  * Return: None
  *
  * This will mask out the supplied interrupt, preventing it from being called
  */
void interrupt_set_mask(uint8_t isr_line);

/**
  * Name:   interrupt_clear_mask
  * Args:   int - The interrupt to be unmasked (0-15 referring to PIC ISR's)
  *
  * Return: None
  *
  * This will unmask the supplied interrupt, allowing it to be triggered
  */
void interrupt_clear_mask(uint8_t isr_line);

#endif
