#include<stddef.h>
#include<stdint.h>
#include<stdarg.h>
#include "clock.h"
#include "terminal.h"
#include "keyboard.h"
#include "stdio.h"
#include "scheduler.h"
#include "asm_utils.h"

volatile uint8_t kill = 0;

void print_clock()
{
    // Set the position of the clock
    terminal_pos pos = terminal_get_pos();
    terminal_set_pos(0,60);

    // Format and print the date/time
    datetime date = clock_get_datetime();
    printf("%02d/%02d/%04d %02d:%02d:%02d", (int)date.month, (int)date.day, 
            (int)date.year, (int)date.hour, (int)date.minute, (int)date.second);

    // Reset the position to before
    terminal_set_pos(pos.row, pos.column);
}

// Simple background task to run
void print_pid_and_wait()
{
    int pid = scheduler_get_pid();
    int count = 0;

    while(1)
    {
        // Print the count
        count++;
        terminal_pos pos = terminal_get_pos();
        __cli();
        terminal_set_pos(pid, 70);
        printf("Count: %02d", count);
        terminal_set_pos(pos.row, pos.column);
        __sti();

        clock_delay(50*pid);
        
        // Lets the user end the process from the game process
        if(kill == pid)
            break;
    }
    
    // Reset the kill variable to the idle process
    kill = 0;

    // Clear the count
    terminal_pos pos = terminal_get_pos();
    __cli();
    terminal_set_pos(pid, 70);
    printf("         ");
    terminal_set_pos(pos.row, pos.column);
    __sti();
}

// Simple game process for the user to play while watching background processes
void game()
{
    // Initialize game variables
    uint8_t num = 0;
    uint8_t solved = 1;
    uint8_t guess = 0;

    // Allow the user to type to the console until the scheduler ends us
    terminal_clear_screen();
    terminal_set_pos(1,0);
    while(1)
    {
        // Reset the game
        if(solved)
        {
            num = clock_random() % 256;
            printf("I'm thinking of a number between 0 and 256. Guess it!\n");
            solved = 0;
        }
        
        // Read and evaluate the keyboard input
        char key = keyboard_next_key();

        // Increment the guess
        if(key >= '0' && key <= '9')
        {
            guess *= 10;
            guess += key - '0';
            terminal_putc(key);
        }
        // Decrement the guess
        else if(key == '\b')
        {
            guess /= 10;
            terminal_putc('\b');
        }
        // Enter a guess
        else if(key == '\n')
        {
            terminal_putc(key);

            if(guess > num)
            {
                printf("Too high!\n");
                guess = 0;
            }
            else if(guess < num)
            {
                printf("Too low!\n");
                guess = 0;
            }
            else
            {
                printf("You won! Play again? (press n to quit)\n");
                solved = 1;
                key = keyboard_next_key();

                // Quit
                if(key == 'n' || key == 'N')
                    break;
                else
                    terminal_clear_screen();
            }
        }
        // Create a new task
        else if(key == 'N')
        {
            scheduler_insert(print_pid_and_wait);
        }
        // Let a particular print_pid_and_wait function end
        else if(key == 'R')
        {
            if(guess < 100)
            {
                kill = guess;
                guess = 0;
            }
        }
    }

    printf("You quit :(\n");
}

void schedule()
{
    scheduler_switch_context();
}

void kernel_main(void)
{
    // Initialize the components
    scheduler_init();
    clock_init();
    terminal_init();
    keyboard_init();

    // Initialize callbacks
    clock_register_callback(250, print_clock);
    clock_register_callback(10, schedule);
    
    for(int i = 0; i < 10; i++)
        scheduler_insert(print_pid_and_wait);
    scheduler_insert(game);

    print_clock();
    
    __sti();

    while(1);
}
