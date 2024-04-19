#include <core.h>
#include <stdlib.h>
#include <stdio.h>
#include <cpu.h>
#include <wfi.h>
#include <plat.h>
#include <irq.h>
#include <timer.h>
#include <prefetch.h>

void task() {
    // Interfering task
    while (1) {
        clear_L2_cache((uint64_t)data, DATA_SIZE);
        prefetch_data((uint64_t)data, DATA_SIZE);
    }
}