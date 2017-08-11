#include<stdint.h>
#include "asm_utils.h"
#include "interrupt.h"
#include "clock.h"

#define TICKS(x) (x / 0.8582)
#define CALLBACK_VECTOR_SIZE 256

// We can handle about 58 days of uptime before the ticks overflow
uint32_t volatile ticks;

typedef struct clock_callback {
    uint32_t ticks;
    uint32_t max_ticks;
    void (*func)();
} clock_callback;

clock_callback callback_vector[CALLBACK_VECTOR_SIZE];

void clock_interrupt_handler(uint32_t vector, uint32_t err)
{
    ticks++;

    // Check every callback 
    for(uint16_t i = 0; i < CALLBACK_VECTOR_SIZE; i++)
    {
        clock_callback* callback = &callback_vector[i];

        // Skip callbacks with no max tick (they're empty)
        if(!callback->max_ticks)
            continue;

        // Decrement its counter, then reset and call the function at zero
        callback->ticks--;
        if(!callback->ticks)
        {
            callback->ticks = callback->max_ticks;
            callback->func();
        }
    }

    // Reset the PIC
    __outb(0x20, 0x20);
}

void clock_init()
{
    ticks = 0;

    // Zero the callback vector
    for(uint16_t i = 0; i < CALLBACK_VECTOR_SIZE; i++)
    {
        callback_vector[i].max_ticks = 0;
    }

    // Initialize the clock on channel 0 to interrupt consistently given 2 bytes
    __outb(0x43, 0b00110110);

    // Initializes to have a tick about once every 0.85 ms
    uint16_t cycles = 1024;
    __outb(0x40, cycles & 0xff);
    __outb(0x40, (cycles >> 8) & 0xff);

    interrupt_register(0x20, &clock_interrupt_handler);
    interrupt_clear_mask(0);
}

datetime clock_get_datetime() 
{
    datetime current;
    
    uint8_t interrupts_enabled = __eflags() & 0x200;

    if(interrupts_enabled) __cli();

    // Wait until the RTC is done updating
    do
    {
        __outb(0x70, 0x0A);
    } while(__inb(0x71) & 0x80);

    // Read the current seconds
    __outb(0x70, 0x00);
    current.second = __inb(0x71);

    // Read the current minute
    __outb(0x70, 0x02);
    current.minute = __inb(0x71);

    // Read the current hour
    __outb(0x70, 0x04);
    current.hour = __inb(0x71);

    // Read the current day
    __outb(0x70, 0x07);
    current.day = __inb(0x71);

    // Read the current month
    __outb(0x70, 0x08);
    current.month = __inb(0x71);

    // Read the current year
    __outb(0x70, 0x09);
    current.year = __inb(0x71);

    // Read the current century
    __outb(0x70, 0x32);
    uint8_t century = __inb(0x71);

    // Convert binary time
    __outb(0x70, 0x0b);
    if((__inb(0x71) & 0x04) == 0)
    {
        current.second = (current.second & 0x0f) + (current.second/16)*10;
        current.minute = (current.minute & 0x0f) + (current.minute/16)*10;
        current.hour = (current.hour & 0x0f) + (current.hour/16)*10;
        current.day = (current.day & 0x0f) + (current.day/16)*10;
        current.month = (current.month & 0x0f) + (current.month/16)*10;
        current.year = (current.year & 0x0f) + (current.year/16)*10;
        century = (century & 0x0f) + (century/16)*10;
    }

    current.year += century*100;
    
    if(interrupts_enabled) __sti();

    return current;
}

uint32_t clock_random()
{
    // Multiply the ticks by a large prime number
    return ticks * 14243;
}

void clock_delay(uint32_t ms)
{
    uint32_t volatile future = ticks + TICKS(ms);
    while(ticks < future);
    return;
}

uint16_t clock_register_callback(uint32_t ms, void (*callback)())
{
    uint16_t id = 0;

    // Locate free callback slot
    for(uint16_t i = 0; i < CALLBACK_VECTOR_SIZE; i++)
    {
        if(callback_vector[i].max_ticks == 0)
        {
            id = i;
            break;
        }
    }

    // Set up the callback entry
    callback_vector[id].max_ticks = TICKS(ms);
    callback_vector[id].ticks = callback_vector[id].max_ticks;
    callback_vector[id].func = callback;

    // Give back the id to be used for removing callbacks later
    return id;
}

void clock_remove_callback(uint16_t id)
{
    if(id < CALLBACK_VECTOR_SIZE)
        callback_vector[id].max_ticks = 0;
}
