#ifndef RV32M_H_
#define RV32M_H_
#include "includes.h"
#include "cpu32.h"

#define MUL_INST 0b0000000001
#define MUL_H_INST 0b0010000001
#define MULHSU_INST 0b0100000001
#define MULHU_INST 0b0110000001
#define DIV_INST 0b1000000001
#define DIVU_INST 0b1010000001
#define REM_INST 0b1100000001
#define REMU_INST 0b1110000001

void mul_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2);
void mulh_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2);
void mulhsu_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2);
void mulhu_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2);
void div_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2);
void divu_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2);
void rem_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2);
void remu_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2);

#endif