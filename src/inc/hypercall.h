#ifndef __HYPERCALL_H__
#define __HYPERCALL_H__
#include <core.h>

/* Hypervisor base value (aarch64) */
#define HYPERCALL_BASE_VALUE 0x86000000 | 0x40000000

union memory_request_answer
{
    struct
    {
        uint64_t ack : 1;  // 0 = no, 1 = yes
        uint64_t ttw : 63; // Time to wait in case of no (0 = no waiting, just not prio)
    };
    uint64_t raw;
};

/* Hypervisor actions (TODO Change name) */
enum hypervisor_actions
{
    HC_IPC = 1,
    HC_REQUEST_MEM_ACCESS = 2,
    HC_REVOKE_MEM_ACCESS = 3,
    HC_GET_CPU_ID = 4,
    HC_NOTIFY_CPU = 5,
    HC_EMPTY_CALL = 6,
    HC_REQUEST_MEM_ACCESS_TIMER = 7,
    HC_DISPLAY_STRING = 8,
    HC_MEASURE_IPI = 9,
    HC_REVOKE_MEM_ACCESS_TIMER = 10,
    HC_UPDATE_MEM_ACCESS = 11
};

/* Hypercall with the specified action and arguments */
uint64_t hypercall(enum hypervisor_actions, uint64_t arg0, uint64_t arg1, uint64_t arg2);

/* Macros for memory request and revoke */
#define request_memory_access(prio) hypercall(HC_REQUEST_MEM_ACCESS, prio, 0, 0)
#define revoke_memory_access() hypercall(HC_REVOKE_MEM_ACCESS, 0, 0, 0)
#define update_memory_access(prio) hypercall(HC_UPDATE_MEM_ACCESS, prio, 0, 0)

#endif