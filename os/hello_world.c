#include<stddef.h>
#include<stdint.h>

static uint16_t* vga_buffer = (uint16_t*)0xb8000;
static uint8_t buffer_pos = 0;
static uint8_t black_on_white = 15;

// Clear the screen
void clear_screen()
{
    // Terminal dimensions
    uint8_t width = 80;
    uint8_t height = 25;

    // Replace every character on the screen with a space
    for(uint8_t x = 0; x < width; x++)
    {
        for(uint8_t y = 0; y < height; y++)
        {
            vga_buffer[y*width + x] = ' ' | black_on_white << 8;
        }
    }
}

// Print a string to the terminal
void print(char* str)
{
    // Print character by character until the 0-terminated string ends
    uint8_t str_pos = 0;
    while(str[str_pos])
    {
        vga_buffer[buffer_pos++] = str[str_pos++] | black_on_white << 8;
    }
}

void kernel_main(void)
{
    clear_screen();
    print("Hello world!");
}
