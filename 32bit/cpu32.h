#ifndef CPU32_H_
#define CPU32_H_
#include "components/bus.h"
#include "components/dram.h"
#include "float_types.h"
#include <stdint.h>

#define MTVEC 0x305
#define MEPC 0x341
#define MCAUSE 0x342
#define MTVAL 0x343

typedef struct vCPU32 vCPU32;

typedef void (*InstructionHandler)(vCPU32 *cpu, uint32_t instruction, uint8_t rd);

typedef void (*RTypeInstructionHandler)(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2);

typedef void (*FRTypeInstructionHandler)(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2, uint8_t rm);

typedef void (*FSTypeInstructionHandler)(vCPU32 *cpu, uint16_t imm, uint8_t rs1, uint8_t rs2);

typedef void (*FITypeInstructionHandler)(vCPU32 *cpu, uint16_t imm, uint8_t rd, uint8_t rs1);

typedef void (*FNMTypeInstructionHandler)(vCPU32 *cpu, uint32_t instr, uint8_t rd);

typedef enum privilege {
    M = 3, U = 0
} privilege;

struct vCPU32 {
    uint32_t x[32];
    float32_t f[32];
    uint32_t fcsr;
    uint32_t pc;
    BUS32 bus;
    privilege privilege;

    //CSR-Registers
    //csr[11:0] csr[11:10] -> registers is read/write 00, 01, 10 or read-only 11; csr[9:8] -> encode the lowest privilege level that can access the CSR (M-Mode 11; User_Mode 00)
    // TODO: instructions that access a CSR without appropriate privilege level raise illegal-instruction exceptions
    // TODO: A read/write register might also contain some bits that are read-only, in which case writes to the read only bits are ignored
    // TODO: only debug mode can access 0x7B0-0x7BF while 0x7A0-0x7BF can only be used by debug mode but m-mode can access them
    uint32_t csr[4096];
    //TODO: handle WPRI: writes to these bits need to be ignored and reads can return an arbitrary value
    // TODO: handle WLRL: the value written must fall within a specific set of legal values (illegal values are ignored); when read the value is always legal; can raise an illegal instruction exception
    // TODO: handle WARL: any value can be written; reads are always legal values (illegal values are replaced, clamped or sanitized) but the legal value returned should deterministically depend on the illegal written value and the architectural state of the hart.
    // TODO: if CSR1 changes the allowed range of CSR2, then CSR2 will be set to an UNSPECIFIED-value from among its new legal values, even if the value remains legal
    // TODO: CSR Width Modulation (for now not allowed)
    // TODO: think about how to check the type of csr


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


// exception handler for instruction is not supported
void exceptionHandler(vCPU32 *cpu, uint32_t instruction, uint8_t rd);
void rTypeExceptionHandler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs);
void frTypeExceptionHandler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2, uint8_t rm);
void fsTypeExceptionHandler(vCPU32 *cpu, uint16_t imm, uint8_t rs1, uint8_t rs2);
void fiTypeExceptionHandler(vCPU32 *cpu, uint16_t imm, uint8_t rd, uint8_t rs1);
void fnmTypeExceptionHandler(vCPU32 *cpu, uint32_t instr, uint8_t rd);

#endif