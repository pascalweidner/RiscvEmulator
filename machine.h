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

/*
    @param specs: String of the format RV<bits>I<extensions> (i.e.: "RV32I")
    @brief creates a virtual machine based on the definition given by the specs string
*/
VM32 *create_vm(char *specs);

VM32 *init_vm32(); 


#endif