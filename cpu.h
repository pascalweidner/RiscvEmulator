#ifndef CPU_H
#define CPU_H
#include "includes.h"

typedef struct vCPU32 {
    uint32_t x[32];
    uint32_t pc;
    BUS32 bus;
} vCPU32;

void cpu32_run(vCPU32 *cpu);

void cpu32_fetch(vCPU32 *cpu, uint32_t inst);

int cpu32_execute(vCPU32 * cpu)

vCPU32 *init_vCPU32();

#endif