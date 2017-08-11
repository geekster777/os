#include<stdint.h>
#include "stdio.h"
#include "scheduler.h"

#define MAX_PROCESSES 100
#define STACK_SIZE 0x10000
#define STACK_TOP 0xff0000

volatile void* scheduler_current_context;

volatile pcb processes[MAX_PROCESSES];
volatile uint8_t current_process;
volatile pcb* idle_process;


typedef struct pcb_context
{
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax, vec, err, eip, cs, eflags; 
} pcb_context;

void end_current_process()
{
    // Mark our process for termination by the context switcher
    processes[current_process].state = PS_KILL;

    // Wait until we lose control. In the future, we should give up control.
    while(1);
}

void idle()
{
    // Just spins the wheels until the context switcher relieves us
    // In the future, we should only run this process if there's nothing left
    printf("Idle process! ");
    for(;;);
}

void scheduler_init()
{
    // Zero out all the processes
    current_process = 0;
    for(uint8_t i = 0; i < MAX_PROCESSES; i++)
    {
        processes[i].state = PS_UNUSED;
        processes[i].pid = i;
    }

    // Insert the idle process as our initial one
    scheduler_insert(idle);
    idle_process = &processes[0];
    idle_process->state = PS_IDLE;
    scheduler_current_context = idle_process->context;
}

int scheduler_insert(void (*func))
{
    // Find the next free pcb
    volatile pcb* current = 0;
    for(uint8_t i = 0; i < MAX_PROCESSES; i++)
    {
        if(processes[i].state == PS_UNUSED)
        {
            current = &processes[i];
            break;
        }
    }

    if(current)
    {
        // Reset the stack to its start
        void* current_stack_top = (void*)(STACK_TOP - current->pid*STACK_SIZE);

        // Set the stack up so that func will return and end the process
        *(void**)(current_stack_top - 4) = end_current_process;

        // Correctly make space on the stack for the context 
        current->context = (void*)(current_stack_top - sizeof(pcb_context) - 4);
        volatile pcb_context* context = current->context;

        // The stack and base should let us return to end_current_process
        context->ebp = (uint32_t)current_stack_top;
        context->esp = (uint32_t)current_stack_top - 4;
        
        context->eip = (uint32_t)func; // Function to control this process
        context->cs = 0x08; // Manually enter the GDT code segment
        context->eflags = 0x246; // Default flags with interrupts enabled

        current->state = PS_SLEEPING;
        return current->pid;
    }
    else
        return -1; // No free processes
}

// Can only be called from an interrupt! 
void scheduler_switch_context()
{
    // Don't mark an unused thread 
    if(processes[current_process].state == PS_KILL)
        processes[current_process].state = PS_UNUSED;
    else
        processes[current_process].state = PS_SLEEPING;

    // Save the context that our ISR stubs kindly recorded for us
    processes[current_process].context = scheduler_current_context;

    // Find the next sleeping process
    do
    {
        current_process = (current_process + 1) % MAX_PROCESSES;
    } while(processes[current_process].state != PS_SLEEPING);

    // Set the new process as running, and let the ISR restore its context
    processes[current_process].state = PS_RUNNING;
    scheduler_current_context = processes[current_process].context;
}

// Return the current context's PID 
uint8_t scheduler_get_pid()
{
    return processes[current_process].pid;
}
