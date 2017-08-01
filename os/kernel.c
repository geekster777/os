#include<stddef.h>
#include<stdint.h>
#include "terminal.h"
#include "keyboard.h"

void key_pressed(char key)
{
    terminal_print_at("Pressed:   ", 0, 0);
    terminal_putc_at(key, 0, 9);
}

void key_released(char key)
{
    terminal_print_at("Released: ", 0, 0);
    terminal_putc_at(key, 0, 10);
}

void kernel_main(void)
{
    terminal_init();
    keyboard_init();
    keyboard_register_keypressed(&key_pressed);
    keyboard_register_keyreleased(&key_released);

    terminal_set_pos(1,0);
    while(1)
    {
        terminal_putc(keyboard_next_key());
    }
}
