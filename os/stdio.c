#include<stddef.h>
#include<stdint.h>
#include<stdarg.h>
#include "terminal.h"
#include "stdio.h"

// This will print out digits of a number given a base and character map.
// It must be given a count of the digits, and will truncate the number, 
// printing only the lowest order digits, if the digit_count is smaller than 
// the actual count. It will pad the left with 0's if the count is too large
void print_digits(int num, int base, char* char_map, int digit_count)
{
    if(digit_count > 0)
    {
        print_digits(num / base, base, char_map, digit_count - 1);
        terminal_putc(char_map[num % base]);
    }
}

void printf(char* str, ...)
{
    va_list parameters;
    va_start(parameters, str);
    
    while(str[0] != '\0')
    {
        if(str[0] != '%' || str[1] == '%')
        {
            // Check if this is a special character
            if(str[0] == '%') str++;

            terminal_putc(str[0]);
            str++;
        }
        else
        {
            str++;

            // Check if a padding character was supplied
            char padding_char = ' ';
            if(str[0] == '0')
            {
                padding_char = '0';
                str++;
            }

            // Get the number of spaces the format should occupy
            int width = 0;
            while( (str[0] >= '0') && (str[0] <= '9') )
            {
                int digit = str[0] - '0';
                width = width*10 + digit;
                str++;
            }

            if(str[0] == 's')
            {
                char* arg = va_arg(parameters, char*);
                int length = 0;

                // Get the string length
                while(arg[length]) length++;

                // Truncate or pad the string if a width was provided
                if(width)
                {
                    if(length > width)
                    {
                        length = width;
                    }
                    else
                    {
                        for(int i = 0; i < width - length; i++)
                        {
                            terminal_putc(padding_char);
                        }
                    }
                }
                
                // Print the string
                for(int i = 0; i < length; i++)
                {
                    terminal_putc(arg[i]);
                }

                str++;
            }
            else if(str[0] == 'd' || str[0] == 'x' || str[0] == 'b')
            {
                int num = va_arg(parameters, int);

                // Determine the base and associated character map for the num
                int base = 10;
                char* char_map = "0123456789";
                if(str[0] == 'x')
                {
                    base = 16;
                    char_map = "0123456789abcdef";
                }
                else if(str[0] == 'b')
                {
                    base = 2;
                    char_map = "01";
                }

                // Count the digits
                int digit_count = 0;
                int temp = num;
                do 
                {
                    digit_count++;
                    temp /= base;
                } while(temp);

                // Truncate or pad the number if a width was supplied
                if(width)
                {
                    for(int i = 0; i < width-digit_count; i++)
                    {
                        terminal_putc(padding_char);
                    }
                    if(digit_count > width) digit_count = width;
                }
                
                // 
                print_digits(num, base, char_map, digit_count);
                str++;
            }
        }
    }

    va_end(parameters);
}

