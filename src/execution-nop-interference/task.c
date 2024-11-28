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
#include "prefetch_nop.h"

uint8_t nop_number = 0;
void ipi_handler(unsigned int id)
{
    nop_number += 1;
}

void task()
{
    irq_set_handler(IPI_PAUSE_IRQ, ipi_handler);
    irq_enable(IPI_PAUSE_IRQ);
    irq_set_prio(IPI_PAUSE_IRQ, IRQ_MAX_PRIO);

    // Interfering task
    while (1)
    {
        clear_L2_cache((uint64_t)appdata, MAX_DATA_SIZE);
        prefetch_data_nop((uint64_t)appdata, MAX_DATA_SIZE, &nop_number);
    }
}