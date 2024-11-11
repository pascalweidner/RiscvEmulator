#include "cpu.h"

vCPU32 *init_vCPU32(DRAM32 *dram) {
    vCPU32 *cpu = (vCPU32*)malloc(sizeof(vCPU32));
    cpu->pc = DRAM32_BASE;
    cpu->x[0] = 0;
    init_bus32(cpu->bus, dram);
}