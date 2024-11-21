#include <hypercall.h>
#include <core.h>

uint64_t hypercall(enum hypervisor_actions action, uint64_t arg0, uint64_t arg1, uint64_t arg2) {
    int hypercall_id = HYPERCALL_BASE_VALUE | action;
    int ret;

    __asm__ volatile (
        "hvc #0\n\t"
        : "=r" (ret)
        : "r" (hypercall_id), "r" (arg0), "r" (arg1), "r" (arg2)
    );

    return ret;
}