#include <core.h>
#include <stdlib.h>
#include <stdio.h>
#include <cpu.h>
#include <wfi.h>
#include <plat.h>
#include <irq.h>
#include <timer.h>
#include <prefetch.h>
#include <data.h>

#define CPU_PRIO 1

int suspended = 0;

void suspended_state()
{
    while (suspended)
        ;
}

void ipi_pause_handler()
{
    if (!suspended)
    {
        // Pause task
        suspended = 1;
        suspended_state();
    }
}

void ipi_resume_handler()
{
    if (suspended)
    {
        // Resume task
        suspended = 0;
    }
}

void task()
{
    // Set handlers for IPI pause and resume
    irq_set_handler(IPI_PAUSE_IRQ, ipi_pause_handler);
    irq_set_handler(IPI_RESUME_IRQ, ipi_resume_handler);

    // Enable IPI interruptions for pause and resume
    irq_enable(IPI_PAUSE_IRQ);
    irq_set_prio(IPI_PAUSE_IRQ, IRQ_MAX_PRIO);

    irq_enable(IPI_RESUME_IRQ);
    irq_set_prio(IPI_RESUME_IRQ, IRQ_MAX_PRIO);

    // Interfering task
    while (1)
    {
        // Asking for memory access
        int ack_access = request_memory_access(CPU_PRIO);

        // If access not granted then we suspend task
        if (!ack_access)
        {
            suspended = 1;
            suspended_state();
        }

        // Access granted
        prefetch_memory();
        revoke_memory_access();

        // Clear and reask
        clear_L2_cache((uint64_t)data, DATA_SIZE);
    }
}