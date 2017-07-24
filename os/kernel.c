#include<stddef.h>
#include<stdint.h>
#include "terminal.h"

void kernel_main(void)
{
    terminal_init();

    while(1)
    {
        for(int i = 15; i > 0; i--)
        {
            terminal_set_color(VGA_COLOR_BLACK, i);
            terminal_print("Hello world! ");
        }
    }
}
