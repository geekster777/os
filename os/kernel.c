#include<stddef.h>
#include<stdint.h>
#include<stdarg.h>
#include "clock.h"
#include "terminal.h"
#include "keyboard.h"
#include "stdio.h"

uint8_t color = 0;

void key_pressed(char key)
{
    terminal_pos pos = terminal_get_pos();
    terminal_set_pos(0,0);
    terminal_print("Pressed: ");
    if(key != '\n')
    {
        terminal_putc(key);
        terminal_putc(' ');
    }
    terminal_set_pos(pos.row, pos.column);
}

void key_released(char key)
{
    terminal_pos pos = terminal_get_pos();
    terminal_set_pos(0,0);
    terminal_print("Released: ");
    if(key != '\n')
    {
        terminal_putc(key);
    }
    terminal_set_pos(pos.row, pos.column);
}

void change_color()
{
    terminal_set_color(VGA_COLOR_BLACK, ++color % 16);
}

void print_clock()
{
    // Set the position and color of the clock
    terminal_pos pos = terminal_get_pos();
    terminal_set_pos(0,60);
    terminal_set_color(VGA_COLOR_BLACK, VGA_COLOR_WHITE);

    // Format and print the date/time
    datetime date = clock_get_datetime();
    printf("%02d/%02d/%04d %02d:%02d:%02d", (int)date.month, (int)date.day, 
            (int)date.year, (int)date.hour, (int)date.minute, (int)date.second);

    // Reset the position and color to before
    terminal_set_pos(pos.row, pos.column);
    terminal_set_color(VGA_COLOR_BLACK, color);

}

void kernel_main(void)
{
    // Initialize the components
    clock_init();
    terminal_init();
    keyboard_init();

    // Initialize callbacks
    keyboard_register_keypressed(&key_pressed);
    keyboard_register_keyreleased(&key_released);
    clock_register_callback(1000, change_color);
    clock_register_callback(250, print_clock);
    
    print_clock();

    // Allow the user to type to the console
    terminal_set_pos(1,0);
    while(1)
    {
        terminal_putc(keyboard_next_key());
    }
}
