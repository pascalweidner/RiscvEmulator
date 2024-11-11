#ifndef MACHINE_H
#define MACHINE_H
#include "includes.h"
#include "cpu.h"
#include "bus.h"

// TODO: add union to create a VM32 and VM64
typedef struct VM32 {
    vCPU32 *cpu;
    DRAM32 *dram;
} VM32;

VM32 *init_vm32(); 


#endif