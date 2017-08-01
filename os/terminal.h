/**
  * Small wrapper to the vga memory to allow for printing to the screen
  */

#include<stdint.h>

#ifndef _TERMINAL_H_
#define _TERMINAL_H_

// List of vga colors
enum vga_color 
{
    VGA_COLOR_BLACK=0,
    VGA_COLOR_BLUE=1,
    VGA_COLOR_GREEN=2,
    VGA_COLOR_CYAN=3,
    VGA_COLOR_RED=4,
    VGA_COLOR_MAGENTA=5,
    VGA_COLOR_BROWN=6,
    VGA_COLOR_LIGHT_GRAY=7,
    VGA_COLOR_DARK_GRAY=8,
    VGA_COLOR_LIGHT_BLUE=9,
    VGA_COLOR_LIGHT_GREEN=10,
    VGA_COLOR_LIGHT_CYAN=11,
    VGA_COLOR_LIGHT_RED=12,
    VGA_COLOR_LIGHT_MAGENTA=13,
    VGA_COLOR_LIGHT_BROWN=14,
    VGA_COLOR_WHITE=15
};

/**
  * Name:   terminal_init
  * Args:   None
  * Return: None
  *
  * Initializes and clears the terminal
  */
void terminal_init();

/**
  * Name:   terminal_set_color
  * Args:   vga_color bg - The background color
  *         vga_color fg - The foreground color
  * Return: None
  *
  * Sets the color the terminal will currently print in. Does not retroactively
  * set previously printed characters.
  */
void terminal_set_color(enum vga_color bg, enum vga_color fg);

/**
  * Name:   terminal_clear_screen
  * Args:   None
  * Return: None
  *
  * Clears the terminal. This is a good way to reset the background color after
  * a color change
  */
void terminal_clear_screen();

/**
  * Name:   terminal_putc
  * Args:   char    - The character to print
  *
  * Prints a single character onto the screen
  */
void terminal_putc(char c);

/**
  * Name:   terminal_putc_at
  * Args:   char    - The character to print
  *         uint8_t - The row to place the character in
  *         uint8_t - The column to place the character in
  * Return: None
  *
  * Prints a character in a provided position without changing the current 
  * cursor position
  */
void terminal_putc_at(char c, uint8_t row, uint8_t col);

/**
  * Name:   terminal_print
  * Args:   char* str - The string to print to the screen
  * Return: None
  *
  * Prints a string at the current location of the screen. Ignores newlines
  */
void terminal_print(char* str);

/**
  * Name:   terminal_print_at
  * Args:   char*   - The character to print
  *         uint8_t - The row to place the character in
  *         uint8_t - The column to place the character in
  * Return: None
  *
  * Prints a string in a provided position without changing the current 
  * cursor position
  */
void terminal_print_at(char* str, uint8_t row, uint8_t col);

/**
  * Name:   terminal_print_byte
  * Args:   uint8_t - The byte to print
  * Return: None
  *
  * Prints a byte in hex format (0x##)
  */
void terminal_print_byte(uint8_t c);

/**
  * Name:   terminal_print_byte_at
  * Args:   uint8_t - The character to print
  *         uint8_t - The row to place the character in
  *         uint8_t - The column to place the character in
  * Return: None
  *
  * Prints a byte in hex in a provided position without changing the current 
  * cursor position
  */
void terminal_print_byte_at(uint8_t c, uint8_t row, uint8_t col);

/**
  * Name:   terminal_set_pos
  * Args:   uint8_t - The row to set the cursor to
  *         uint8_t - The column to set the cursor to
  * Return: None
  *
  * Sets the current cursor to a row/column coordinate
  */
void terminal_set_pos(uint8_t row, uint8_t column);

#endif
