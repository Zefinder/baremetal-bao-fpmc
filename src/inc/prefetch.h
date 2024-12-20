#ifndef __PREFETCH_H__
#define __PREFETCH_H__

void clear_L2_cache(uint64_t address, uint64_t size);
void prefetch_data(uint64_t address, uint64_t size, volatile uint8_t *pause);

#endif