#ifndef __PREFETCH_H__
#define __PREFETCH_H__

void clear_L2_cache(unsigned long long address, unsigned long long size);
void prefetch_data(unsigned long long address, unsigned long long size);

#endif