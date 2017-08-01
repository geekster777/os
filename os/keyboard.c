/**
  * Keyboard interface to listen for keypresses
  */

#include<stdint.h>
#include "asm_utils.h"
#include "interrupt.h"
#include "keyboard.h"

#define KEY_BUFFER_SIZE 128
#define KEY_LEFT_SHIFT  42
#define KEY_RIGHT_SHIFT 54

volatile char last_key = 0;
uint8_t shift_enabled;

void (*keyboard_keypressed_callback)(char) = 0;
void (*keyboard_keyreleased_callback)(char) = 0;

unsigned char scan_codes[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8','9', 
    '0', '-', '=', '\b', /* Backspace */
    '\t', /* Tab */
    'q', 'w', 'e', 'r',/* 19 */
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
    0, /* 29   - Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
    '\'', '`',   0, /* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', /* 49 */
    'm', ',', '.', '/',   0, /* Right shift */
    '*',
    0, /* Alt */
    ' ', /* Space bar */
    0, /* Caps lock */
    0, /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0, /* < ... F10 */
    0, /* 69 - Num lock*/
    0, /* Scroll Lock */
    0, /* Home key */
    0, /* Up Arrow */
    0, /* Page Up */
    '-',
    0, /* Left Arrow */
    0,
    0, /* Right Arrow */
    '+',
    0, /* 79 - End key*/
    0, /* Down Arrow */
    0, /* Page Down */
    0, /* Insert Key */
    0, /* Delete Key */
    0,   0,   0,
    0, /* F11 Key */
    0, /* F12 Key */
    0  /* All other keys are undefined */
};

unsigned char shift_scan_codes[128] =
{
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*','(', 
    ')', '_', '+', '\b', /* Backspace */
    '\t', /* Tab */
    'Q', 'W', 'E', 'R',/* 19 */
    'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', /* Enter key */
    0, /* 29   - Control */
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', /* 39 */
    '\"', '~',   0, /* Left shift */
    '|', 'Z', 'X', 'C', 'V', 'B', 'N', /* 49 */
    'M', '<', '>', '?',   0, /* Right shift */
    '*',
    0, /* Alt */
    ' ', /* Space bar */
    0, /* Caps lock */
    0, /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0, /* < ... F10 */
    0, /* 69 - Num lock*/
    0, /* Scroll Lock */
    0, /* Home key */
    0, /* Up Arrow */
    0, /* Page Up */
    '-',
    0, /* Left Arrow */
    0,
    0, /* Right Arrow */
    '+',
    0, /* 79 - End key*/
    0, /* Down Arrow */
    0, /* Page Down */
    0, /* Insert Key */
    0, /* Delete Key */
    0,   0,   0,
    0, /* F11 Key */
    0, /* F12 Key */
    0  /* All other keys are undefined */
};

// Handles all keyboard interrupts, and triggers appropriate callbacks
void keyboard_interrupt_handler(uint16_t vector)
{
    uint8_t code = __inb(0x60);
    unsigned char* codes; 

    // Check if shift is being pressed or released
    if(code == KEY_RIGHT_SHIFT)
        shift_enabled |= 0x01; // Enable
    else if(code == KEY_LEFT_SHIFT)
        shift_enabled |= 0x02; // Enable
    else if((code & ~0x80) == KEY_RIGHT_SHIFT)
        shift_enabled &= ~0x01; // Disable
    else if((code & ~0x80) == KEY_LEFT_SHIFT)
        shift_enabled &= ~0x02; // Disable

    // Enable shift appropriate scan codes
    codes = shift_enabled ? shift_scan_codes : scan_codes;

    // Key released
    if(code & 0x80)
    {
        char key = codes[ code & ~0x80 ];
        if(keyboard_keyreleased_callback) keyboard_keyreleased_callback(key);
    }
    // Key pressed
    else
    {
        char key = codes[ code ];
        last_key = key;
        if(keyboard_keypressed_callback) keyboard_keypressed_callback(key);
    }

    // Tell the PIC to re-enable interrupts
    __outb(0x20, 0x20);
}

// Initializes the keyboard
void keyboard_init()
{
    // Register an initial keyboard handler to be overwritten
    interrupt_register(0x21, &keyboard_interrupt_handler);

    // Empty keyboard buffer to make sure interrupts work    
    __inb(0x60);
}

// Reads the current key from the keyboard
char keyboard_get_key()
{
    // read the next key
    uint8_t code = __inb(0x60);

    if(code & 0x80)
    {
        return 0;
    }
    else
    {
        return scan_codes[code];
    }
}

// Waits for the next keystroke from the keyboard (or immediately reads from
// the buffer)
char keyboard_next_key()
{
    // Wait until the key changes
    while(last_key == 0);

    // Reset the key so we can wait for the next one in the future
    char key = last_key;
    last_key = 0;
    return key;
}

// Registers a callback to be triggered when a key is pressed
void keyboard_register_keypressed(void (*callback)(char))
{
    keyboard_keypressed_callback = callback;
}

// Registers a callback to be triggered when a key is released
void keyboard_register_keyreleased(void (*callback)(char))
{
    keyboard_keyreleased_callback = callback;
}
