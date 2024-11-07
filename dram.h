#include "includes.h"

#define DRAM_SIZE = 1024 * 1024 * 1;
#define DRAM32_BASE = 0x80000000

typedef struct DRAM32 {
    uint8_t mem[DRAM_SIZE];
} DRAM32;

uint32_t dram32_load(DRAM32 *dram, uint32_t addr, uint32_t size);
void dram32_store(DRAM32 *dram, uint32_t addr, uint32_t size, uint32_t value);

