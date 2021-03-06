#include<stdint.h>
#include<stddef.h>
#include "terminal.h"

#define TERMINAL_HEIGHT 25
#define TERMINAL_WIDTH 80

static uint16_t* terminal_buffer = (uint16_t*)0xb8000;
static uint16_t terminal_buffer_pos = 0;
static uint8_t terminal_color = 0x0F;

// Creates a console object incorporating characters and color
uint16_t make_char(char letter, uint8_t color)
{
    return letter | color << 8;
}

// Helper function to create a color integer from an fg and bg
uint8_t make_color(enum vga_color background, enum vga_color foreground)
{
    return background << 4 | (foreground & 0xf);
}

// Initialize the terminal with a color and clear the screen
void terminal_init()
{
    terminal_buffer_pos = 0;
    terminal_set_color(VGA_COLOR_BLACK, VGA_COLOR_WHITE);
    terminal_clear_screen();
}

// Clear the screen
void terminal_clear_screen()
{
    // Replace every character on the screen with a space
    for(uint8_t x = 0; x < TERMINAL_WIDTH; x++)
    {
        for(uint8_t y = 0; y < TERMINAL_HEIGHT; y++)
        {
            terminal_buffer[y*TERMINAL_WIDTH + x] 
                = make_char(' ', terminal_color);
        }
    }

    // Reset terminal coordinates
    terminal_buffer_pos = 0;
}

// Set the colors of the terminal
void terminal_set_color(enum vga_color background, enum vga_color foreground)
{
    terminal_color = make_color(background, foreground);
}

// Scrolls the terminal down while preserving the screen properly
void terminal_scroll_down()
{
    terminal_buffer_pos = (terminal_buffer_pos > TERMINAL_WIDTH) ?
        terminal_buffer_pos - TERMINAL_WIDTH : 0;
    
    uint16_t offset = 0;
    while(offset < TERMINAL_WIDTH * (TERMINAL_HEIGHT-1))
    {
        terminal_buffer[offset] = terminal_buffer[offset + TERMINAL_WIDTH];
        offset++;
    }
    for(int i = 0; i < TERMINAL_WIDTH; i++)
    {
        terminal_buffer[offset + i] = make_char(' ', terminal_color);
    }
}

// Places a character on the screen in the current buffer position
void terminal_putc(char c)
{
    switch(c)
    {
        case '\n':
            terminal_buffer_pos += TERMINAL_WIDTH;
            terminal_buffer_pos -= terminal_buffer_pos % TERMINAL_WIDTH;
            break;
        case '\b':
            if(terminal_buffer_pos > 0)
            {
                terminal_buffer_pos--;
                terminal_buffer[terminal_buffer_pos] = ' ';
            }
            break;
        default:
            terminal_buffer[terminal_buffer_pos++]
                = make_char(c, terminal_color);
    }
}

// Print a string to the terminal
void terminal_print(char* str)
{
    // Print character by character until the 0-terminated string ends
    uint8_t str_pos = 0;
    while(str[str_pos])
    {
        terminal_buffer[terminal_buffer_pos++] 
            = make_char(str[str_pos++], terminal_color);
        
        // Have the page scroll at the end of the screen
        if(terminal_buffer_pos >= TERMINAL_HEIGHT*TERMINAL_WIDTH)
        {
            terminal_scroll_down();
        }
    }
}

// Print a single byte in hex
void terminal_print_byte(uint8_t c)
{
    terminal_print("0x");
    
    char upper = "0123456789abcdef"[(uint8_t)(c >> 4)];
    char lower = "0123456789abcdef"[(uint8_t)(c & 0x0f)];

    terminal_buffer[terminal_buffer_pos++] = make_char(upper, terminal_color);
    terminal_buffer[terminal_buffer_pos++] = make_char(lower, terminal_color);
}

// Sets the buffer position when given a row/column
void terminal_set_pos(uint8_t row, uint8_t column)
{
    if((row < TERMINAL_HEIGHT) && (column < TERMINAL_WIDTH))
    {
        terminal_buffer_pos = row * TERMINAL_WIDTH + column;
    }
}

// Return the current buffer position as a row/column pair
terminal_pos terminal_get_pos()
{
    terminal_pos position;
    position.row = terminal_buffer_pos / TERMINAL_WIDTH;
    position.column = terminal_buffer_pos % TERMINAL_WIDTH;
    return position;
}
