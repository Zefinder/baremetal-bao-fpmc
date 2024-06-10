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

void task()
{
    uint8_t pause = 0;
    // Interfering task
    while (1)
    {
        clear_L2_cache((uint64_t)appdata, MAX_DATA_SIZE);
        prefetch_data((uint64_t)appdata, MAX_DATA_SIZE, &pause);
    }
}