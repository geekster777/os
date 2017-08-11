/**
  * Defines the process queue, storing relevant information for each process
  */

#include<stdint.h>

#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

enum PROCESS_STATE 
{
    PS_RUNNING,
    PS_SLEEPING,
    PS_UNUSED,
    PS_KILL,
    PS_IDLE
};

typedef struct pcb
{
    uint8_t pid;
    uint8_t state;
    void* context;
} pcb;

/**
  * Name:   scheduler_init
  * Args:   None
  * Return: None
  *
  * Initializes the scheduler to zero out all the PCB's and initialize an idle
  * process.
  */
void scheduler_init();

/**
  * Name:   scheduler_insert
  * Args:   void func() - The function to be run in the new process
  * Return: int - The new process's PID, or an error code if negative
  *
  * Starts a new process that will run the provided function
  */
int scheduler_insert(void (*func));

/**
  * Name:   scheduler_switch_context
  * Args:   None
  * Return: None
  *
  * Changes the current process, which the current ISR will restore to. This
  * function can only be called from within an ISR, otherwise things will get
  * wonky.
  */
void scheduler_switch_context();

/**
  * Name:   scheduler_get_pid
  * Args:   None
  * Returns: uint8_t - The PID of the current process
  *
  * Returns the PID of the current process. The numebr can be used to identify
  * processes.
  */
uint8_t scheduler_get_pid();

#endif
