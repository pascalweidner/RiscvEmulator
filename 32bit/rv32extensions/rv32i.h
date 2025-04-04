#ifndef CPU_H
#define CPU_H
#include <includes.h>
#include "../cpu32.h"

#define RTYPE 0b0110011
#define STYPE 0b0100011
#define ITYPE 0b0000011
#define ITYPE2 0b0010011
#define ITYPE3 0b1100111
#define JTYPE 0b1101111
#define UTYPE 0b0010111
#define UTYPE2 0b0110111
#define BTYPE 0b1100011
#define EBREAK 0b1110011

#define ADD_INST 0
#define SUB_INST 0b0000100000
#define SLL_INST 0b0010000000
#define SLT_INST 0b0100000000
#define SLTU_INST 0b0110000000
#define XOR_INST 0b1000000000
#define SRL_INST 0b1010000000
#define SRA_INST 0b1010100000
#define OR_INST 0b1100000000
#define AND_INST 0b1110000000


void add_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2);
void sub_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2);
void sll_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2);
void slt_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2);
void sltu_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2);
void xor_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2);
void srl_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2);
void sra_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2);
void or_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2);
void and_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2);

#endif