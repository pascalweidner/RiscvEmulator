#ifndef MACHINE_H
#define MACHINE_H
#include "includes.h"
#include "bus.h"

typedef void (*InstructionHandler)(vCPU32 *cpu, uint32_t instruction, uint8_t rd);

typedef void (*RTypeInstructionHandler)(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2);

// TODO: add union to create a VM32 and VM64
typedef struct VM32 {
    vCPU32 *cpu;
    DRAM32 *dram;
    InstructionHandler table[255];
    RTypeInstructionHandler rtypeTable[2047];
} VM32;

/*
    @param specs: String of the format RV<bits>I<extensions> (i.e.: "RV32I")
    @brief creates a virtual machine based on the definition given by the specs string
*/
VM32 *create_vm(char *specs);

#endif