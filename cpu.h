#ifndef CPU_H
#define CPU_H
#include "includes.h"
#include "bus.h"
#include "dram.h"

typedef struct vCPU32 {
    uint32_t x[32];
    uint32_t pc;
    BUS32 bus;
} vCPU32;

void cpu32_run(vCPU32 *cpu);

vCPU32 *init_vCPU32(DRAM32 *dram);

#endif