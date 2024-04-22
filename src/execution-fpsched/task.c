#include <core.h>
#include <stdlib.h>
#include <stdio.h>
#include <cpu.h>
#include <wfi.h>
#include <plat.h>
#include <irq.h>
#include <timer.h>
#include <prefetch.h>
#include <hypercall.h>
#include <data.h>

#define CPU_PRIO 1

uint8_t pause = 0;

void ipi_pause_handler()
{
    if (!pause)
    {
        // Pause task
        pause = 1;
        // printf("Suspended\n");
    }
}

void ipi_resume_handler()
{
    if (pause)
    {
        // Resume task
        pause = 0;
        // printf("Resumed\n");
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
            pause = 1;
        }

        // Access granted
        clear_L2_cache((uint64_t)data, DATA_SIZE);
        prefetch_data((uint64_t)data, DATA_SIZE, &pause);
        revoke_memory_access();

        // Clear and reask
    }
}