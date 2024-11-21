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
#include <generic_timer.h>

uint8_t pause = 0;

void ipi_pause_handler()
{
    if (!pause)
    {
        // Pause task
        pause = 1;
    }
}

void ipi_resume_handler()
{
    if (pause)
    {
        // Resume task
        pause = 0;
    }
}

uint64_t cpu_id = 0;
uint64_t lowprio_time = 0;
void timer_handler()
{
    if (lowprio_time != 0)
    {
        uint64_t current_time = generic_timer_read_counter();
        if (current_time >= lowprio_time)
        {
            lowprio_time = 0;

            // Time to wait over, just yes or no! 
            union memory_request_answer update = {.raw = update_memory_access(cpu_id)};
            pause = ~update.ack;
        }
    }
}

void task()
{
    // Set handlers for IPI pause and resume and timer
    irq_set_handler(IPI_PAUSE_IRQ, ipi_pause_handler);
    irq_set_handler(IPI_RESUME_IRQ, ipi_resume_handler);
    irq_set_handler(TIMER_IRQ_ID, timer_handler);

    // Enable IPI interruptions for pause and resume
    irq_enable(IPI_PAUSE_IRQ);
    irq_set_prio(IPI_PAUSE_IRQ, IRQ_MAX_PRIO);

    irq_enable(IPI_RESUME_IRQ);
    irq_set_prio(IPI_RESUME_IRQ, IRQ_MAX_PRIO);

    // Enable timer interrupts (every 1ms)
    timer_set(TIME_MS(1));
    irq_enable(TIMER_IRQ_ID);
    irq_set_prio(TIMER_IRQ_ID, IRQ_MAX_PRIO);

    uint64_t cpu_id = hypercall(HC_GET_CPU_ID, 0, 0, 0);
    // Interfering task
    while (1)
    {
        clear_L2_cache((uint64_t)appdata, MAX_DATA_SIZE);

        // Asking for memory access
        union memory_request_answer ack_access = {.raw = hypercall(HC_REQUEST_MEM_ACCESS, 1 + (2 * cpu_id - 1), 272079, 0)};

        // If access not granted then we suspend task
        pause = ~ack_access.ack;

        // Set time in low prio
        lowprio_time = generic_timer_read_counter() + ack_access.ttw;

        // Access granted
        prefetch_data((uint64_t)appdata, MAX_DATA_SIZE, &pause);

        // Revoke access
        lowprio_time = 0;
        revoke_memory_access();
    }
}