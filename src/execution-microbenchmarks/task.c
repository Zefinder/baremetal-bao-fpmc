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
#include <hypercall.h>

void empty_handler(unsigned int id)
{
}

void task()
{
    irq_set_handler(IPI_PAUSE_IRQ, empty_handler);
    irq_enable(IPI_PAUSE_IRQ);
    irq_set_prio(IPI_PAUSE_IRQ, IRQ_MAX_PRIO);

    irq_set_handler(IPI_RESUME_IRQ, empty_handler);
    irq_enable(IPI_RESUME_IRQ);
    irq_set_prio(IPI_RESUME_IRQ, IRQ_MAX_PRIO);

    uint64_t cpuid = hypercall(HC_GET_CPU_ID, 0, 0, 0);

    // Just ask for access and revoke
    while (1)
    {
        request_memory_access(1 + 2 * (cpuid - 1));
        revoke_memory_access();
    }
}