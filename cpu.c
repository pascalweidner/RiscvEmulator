#include "cpu.h"

vCPU32 *init_vCPU32(DRAM32 *dram) {
    vCPU32 *cpu = (vCPU32*)malloc(sizeof(vCPU32));
    cpu->pc = DRAM32_BASE;
    cpu->x[0] = 0;
    init_bus32(cpu->bus, dram);
}

void cpu32_run(vCPU32 *cpu) {

}

static void cpu32_fetch(vCPU32 *cpu, uint32_t inst) {
    uint8_t inst = bus32_load_dram(&cpu->bus, cpu->pc, 32);

}

static int cpu32_execute(vCPU32 *cpu) {
    //TODO: decode the instruction and execute the operation
}