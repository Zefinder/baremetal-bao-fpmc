#include <prefetch_inc.h>

.global prefetch_data_nop
.type prefetch_data_nop, %function
.section .text

/*
 * void prefetch_data_nop(address, size, pause)
 * 
 * Prefetches [size] bytes from address [address]. Each prefetch is separated
 * by *[nop_number] nops
 * 
 * x0: uint64_t address
 * x1: uint64_t size 
 * x2: uint8_t *nop_number 
 */
.func 
prefetch_data_nop:
    lsr x1, x1, #LOG2_L2_CACHE_LINE_SIZE    // x1 is now the remaining number of lines to prefetch
    ldr x2, [x2]                            // Set x2 to the number of nop
    mov x3, #0                              // x3 is the nop counter, init to 0

nop_loop:
    nop             // Just do nothing
    add x3, x3, #1  // x3 += 1
    cmp x3, x2      // while x3 < x2 continue to nop
    beq nop_loop        

prefetch_loop:
    mov x3, #0                      // x3 = 0
    prfm PLDL2KEEP, [x0]            // Prefetch L2 cache from address [x0 & ~(L2_CACHE_LINE_SIZE - 1) ; x0 | (L2_CACHE_LINE_SIZE - 1)]
    add x0, x0, #L2_CACHE_LINE_SIZE // x0 += L2_CACHE_LINE_SIZE
    subs x1, x1, #1                 // x1 -= 1
    bne nop_loop                    // If x1 > 0 continue  

end_prefetch:
    ret

.endfunc
