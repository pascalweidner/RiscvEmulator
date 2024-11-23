#include "rv32i.h"

void add_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2) {
    cpu->x[rd] = (int32_t)(cpu->x[rs1]) + (int32_t)(cpu->x[rs2]);
}

void sub_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2) {
    cpu->x[rd] = (int32_t)(cpu->x[rs1]) + (int32_t)(cpu->x[rs2]);
}

void sll_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2) {
    cpu->x[rd] = cpu->x[rs1] << (cpu->x[rs2] & 0b1111);
}

void slt_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2) {
    cpu->x[rd] = (int32_t)(cpu->x[rs1]) < (int32_t)(cpu->x[rs2]);
}

void sltu_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2) {
    cpu->x[rd] = (uint32_t)(cpu->x[rs1]) < (uint32_t)(cpu->x[rs2]);
}

void xor_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2) {
    cpu->x[rd] = cpu->x[rs1] ^ cpu->x[rs2];
}

void srl_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2) {
    cpu->x[rd] = cpu->x[rs1] >> (cpu->x[rs2] & 0b1111);
}

void sra_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2) {
    cpu->x[rd] = (int32_t)(cpu->x[rs1]) >> (cpu->x[rs2] & 0b1111);
}

void or_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2) {
    cpu->x[rd] = cpu->x[rs1] | cpu->x[rs2];
}

void and_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2) {
    cpu->x[rd] = cpu->x[rs1] & cpu->x[rs2];
}
