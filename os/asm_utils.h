/**
  * Assembly utilities to be called by C code
  */

#ifndef _ASMUTILS_H_
#define _ASMUTILS_H_

/**
  * Name:   __outb, __outw, __outl
  * Args:   int - port number to write to
  *         int - value value to write to the port
  * Return: None
  *
  * Write value to a port for port-based io
  */
void __outb(int port, int value);
void __outw(int port, int value);
void __outl(int port, int value);


/**
  * Name:   __inb, __inw, __inl
  *
  * Args:   int - The port to read from
  *
  * Return: int - The value read from the port
  *
  * Read value from a port for port-based io
  */
int __inb(int port);
int __inw(int port);
int __inl(int port);

/**
  * Name: cli
  *
  * Disables interrupts by running the cli command
  */
void __cli();

/**
  * Name: sti
  *
  * Enables interrupts by running the sti command
  */
void __sti();

#endif
