#ifndef __PREFETCH_NOP_H__
#define __PREFETCH_NOP_H__

void prefetch_data_nop(uint64_t address, uint64_t size, uint8_t *nop_number);

#endif