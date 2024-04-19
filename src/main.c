/** 
 * Bao, a Lightweight Static Partitioning Hypervisor 
 *
 * Copyright (c) Bao Project (www.bao-project.org), 2019-
 *
 * Authors:
 *      Jose Martins <jose.martins@bao-project.org>
 *      Sandro Pinto <sandro.pinto@bao-project.org>
 *
 * Bao is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License version 2 as published by the Free
 * Software Foundation, with a special exception exempting guest code from such
 * license. See the COPYING file in the top-level directory for details. 
 *
 */

#include <core.h>
#include <stdlib.h>
#include <stdio.h>
#include <cpu.h>
#include <wfi.h>
#include <plat.h>
#include <irq.h>
#include <timer.h>
#include <prefetch.h>

#define TIMER_INTERVAL (TIME_S(1))

#define DATA_SIZE 512*1024
uint8_t data[DATA_SIZE] = {2};

// void ipi_handler(){
//     printf("cpu%d: %s\n", get_cpuid(), __func__);
// }

// void timer_handler(){
//     printf("cpu%d: %s\n", get_cpuid(), __func__);
//     timer_set(TIMER_INTERVAL);
// }

extern void task(void);

void main(void){
    // Greetings (useless but I like it)
    printf("Bao bare-metal test guest\n");

    // Set handlers for timer and IPI
    // irq_set_handler(TIMER_IRQ_ID, timer_handler);
    // irq_set_handler(IPI_IRQ_ID, ipi_handler);

    // Set timer value and enable timer interruption (useless for now but keep for later?)
    // timer_set(TIMER_INTERVAL);
    // irq_enable(TIMER_IRQ_ID);
    // irq_set_prio(TIMER_IRQ_ID, IRQ_MAX_PRIO); 

    // Enable IPI interruption (useless for now but keep for later?)
    // irq_enable(IPI_IRQ_ID);
    // irq_set_prio(IPI_IRQ_ID, IRQ_MAX_PRIO);

    task();

    // Should not come here since task should have a while(1)
    while(1) wfi();
}
