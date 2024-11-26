#include "includes.h"
#include "rv32m.h"

void mul_handler(vCPU32* cpu, uint8_t rd, uint8_t rs1, uint8_t rs2) {
    cpu->x[rd] = ((int32_t)(cpu->x[rs1]) * (int32_t)(cpu->x[rs2])) & 0xFFFFFFFF;
}

void mulh_handler(vCPU32* cpu, uint8_t rd, uint8_t rs1, uint8_t rs2) {
    cpu->x[rd] = (int64_t)((int32_t)(cpu->x[rs1]) * (int32_t)(cpu->x[rs2])) >> 32;
}

void mulhsu_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2) {
    cpu->x[rd] = (int64_t)((int32_t)(cpu->x[rs1]) * (uint32_t)(cpu->x[rs2])) >> 32;
}

void mulhu_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2) {
    cpu->x[rd] = (uint64_t)((uint32_t)(cpu->x[rs1]) * (uint32_t)(cpu->x[rs2])) >> 32;
}

void div_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2) {
    cpu->x[rd] = (int32_t)(cpu->x[rs1]) / (int32_t)(cpu->x[rs2]);
}

void divu_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2) {
    cpu->x[rd] = cpu->x[rs1] / cpu->x[rs2];
}

void rem_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2) {
    cpu->x[rd] = (int32_t)(cpu->x[rs1]) % (int32_t)(cpu->x[rs2]);
}

void remu_handler(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2) {
    cpu->x[rd] = cpu->x[rs1] % cpu->x[rs2];
}


