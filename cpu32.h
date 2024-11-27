#ifndef CPU32_H_
#define CPU32_H_
#include "bus.h"
#include "dram.h"
#include "float_types.h"

typedef struct vCPU32 vCPU32;

typedef void (*InstructionHandler)(vCPU32 *cpu, uint32_t instruction, uint8_t rd);

typedef void (*RTypeInstructionHandler)(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2);

typedef void (*FRTypeInstructionHandler)(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2, uint8_t rm);

typedef void (*FSTypeInstructionHandler)(vCPU32 *cpu, uint16_t imm, uint8_t rs1, uint8_t rs2);

typedef void (*FITypeInstructionHandler)(vCPU32 *cpu, uint16_t imm, uint8_t rd, uint8_t rs1);

struct vCPU32 {
    uint32_t x[32];
    float32_t f[32];
    uint32_t pc;
    BUS32 bus;
    InstructionHandler *table;
    RTypeInstructionHandler *rtypeTable;
    FRTypeInstructionHandler  *frtypeTable;
    FSTypeInstructionHandler *fstypeTable;
    FITypeInstructionHandler *fitypeTable;
};

void cpu32_run(vCPU32 *cpu);

vCPU32 *init_vCPU32(DRAM32 *dram, InstructionHandler *table, RTypeInstructionHandler *rtypeTable);

void rtype(vCPU32* cpu, uint32_t inst, uint8_t rd);
void stype(vCPU32* cpu, uint32_t inst, uint8_t imm1);
void btype(vCPU32* cpu, uint32_t inst, uint8_t imm1);
void utype(vCPU32* cpu, uint32_t inst, uint8_t rd);
void utype2(vCPU32 * cpu, uint32_t inst, uint8_t rd);
void itype(vCPU32 *cpu, uint32_t inst, uint8_t rd);
void itype2(vCPU32 *cpu, uint32_t inst, uint8_t rd);
void itype3(vCPU32 *cpu, uint32_t inst, uint8_t rd);
void jtype(vCPU32 *cpu, uint32_t inst, uint8_t rd);
//TODO: implement ebreak
void ebreak(vCPU32 *cpu, uint32_t inst, uint8_t rd);

// standard decoding functions for the F-Extension
void frtype(vCPU32 *cpu, uint32_t inst, uint8_t rd);
void fitype(vCPU32 *cpu, uint32_t inst, uint8_t rd);
void fstype(vCPU32 *cpu, uint32_t inst, uint8_t rd);
void fmadd(vCPU32 *cpu, uint32_t inst, uint8_t rd);
void fmsub(vCPU32 *cpu, uint32_t inst, uint8_t rd);
void fnmsub(vCPU32 *cpu, uint32_t inst, uint8_t rd);
void fnmadd(vCPU32 *cpu, uint32_t inst, uint8_t rd);




#endif