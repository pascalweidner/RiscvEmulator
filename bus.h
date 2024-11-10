#ifndef BUS_H
#define BUS_H
#include "includes.h"
#include "dram.h"

typedef struct BUS32 {
    DRAM32 *dram;
} BUS32;

void bus32_store_dram(BUS32 *bus, uint32_t addr, uint32_t size); 
uint32_t bus32_load_dram(BUS32 *bus, uint32_t addr, uint32_t size);

BUS32 *init_bus32(BUS32 *bus, DRAM32 *dram);

#endif BUS_H