
#include <sysregs.h>
#include <plat.h>

#define STACK_SIZE  0x4000
#define GICV3 (3)

.section .start, "ax"
.global _start
_start:

    mrs r0, cpsr
    and r1, r0, #CPSR_M_MSK
    cmp r1, #CPSR_M_HYP
    beq 1f
    cps #MODE_SYS
    b entry_el1
1:
#if GIC_VERSION == GICV3
    mrc p15, 4, r0, c12, c9, 5 // icc_hsre
    orr r0, r0, #(1 << 3)
    mcr p15, 4, r0, c12, c9, 5 // icc_hsre
#endif

    mrs r0, cpsr
    mov r1, #MODE_SYS
    bfi r0, r1, #0, #5
    msr spsr_hyp, r0
    ldr r0, =entry_el1
    msr elr_hyp, r0
    dsb
    isb
    eret

entry_el1:
    mrc p15, 0, r0, c0, c0, 5 // mpidr
    and r0, r0, #MPIDR_CPU_MASK

    // TODO: check if we are at EL1/PL1 else jump to it

    ldr r1, =_exception_vector
    mcr	p15, 0, r1, c12, c0, 0 // vbar

    // TODO: setup translation and page-tables

    // TODO: enable floating point

    // TODO: invalidate caches, bp, etc...

#ifdef MPU
    ldr r4, =MAIR_EL1_DFLT
    mcr p15, 0, r4, c10, c2, 0 // mair

    // Set MPU region for cacheability and shareability
    mov r4, #0
    mcr p15, 0, r4, c6, c2, 1  // prselr
    mov r4, #(PRBAR_BASE(0) | PRBAR_SH_IS | PRBAR_AP_RW_ALL)
    mcr p15, 0, r4, c6, c3, 0  // prbar
    mov r4, #(PRLAR_LIMIT(0x7fffffffUL) | PRLAR_ATTR(1) | PRLAR_EN)
    mcr p15, 0, r4, c6, c3, 1  // prlar
#endif

    dsb
    isb

    ldr r1, =(SCTLR_RES1 | SCTLR_C | SCTLR_I | SCTLR_BR | SCTLR_M)
    mcr p15, 0, r1, c1, c0, 0 // sctlr

	dsb	nsh
	isb

    cmp r0, #0
    bne 1f

    ldr r11, =__bss_start 
    ldr r12, =__bss_end
    bl  clear

    .pushsection .data
    .balign 4
wait_flag:
    .word 0x0
    .popsection

    ldr r1, =wait_flag
    mov r2, #1
    str r2, [r1]
1:
    ldr r1, =wait_flag
    ldr r2, [r1]
    cmp r2, #0
    beq 1b

    ldr r1, =_stack_base
    ldr r2, =STACK_SIZE
    add r1, r1, r2
#ifndef SINGLE_CORE
    mul r2, r2, r0
    add r1, r1, r2
#endif
    mov sp, r1

    // TODO: other c runtime init (eg ctors)

    bl _init
    b _exit

.global psci_wake_up
psci_wake_up:
    b .

 .func clear
clear:
    mov r10, #0
2:
	cmp	r11, r12			
	bge 1f				
	str	r10, [r11]
    add r11, r11, #4
	b	2b				
1:
	bx lr
.endfunc