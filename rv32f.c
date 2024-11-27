#include "rv32f.h"
#include <fenv.h>
#include <math.h>

static inline void setRM(uint8_t rm) {
    switch(rm) {
        case 0: // RNE
            fesetround(FE_TONEAREST);
            break;
        case 1: // RTZ
            fesetround(FE_TOWARDZERO);
            break;
        case 2: // RDN
            fesetround(FE_DOWNWARD);
            break;
        case 3: // RUP
            fesetround(FE_UPWARD);
            break;
        default:
            printf("Invalid rounding mode!\n");
            exit(-1);
    }
}

void fadd_handler(vCPU32 *cpu, uint8_t rd, uint8_t rm, uint8_t rs1, uint8_t rs2) {
    setRM(rm);

    cpu->f[rd] = cpu->f[rs1] + cpu->f[rs2]; 
}

void fsub_handler(vCPU32 *cpu, uint8_t rd, uint8_t rm, uint8_t rs1, uint8_t rs2) {
    setRM(rm);

    cpu->f[rd] = cpu->f[rs1] - cpu->f[rs2];
}

void fmul_handler(vCPU32 *cpu, uint8_t rd, uint8_t rm, uint8_t rs1, uint8_t rs2) {
    setRM(rm);

    cpu->f[rd] = cpu->f[rs1] - cpu->f[rs2];
}

void fdiv_handler(vCPU32 *cpu, uint8_t rd, uint8_t rm, uint8_t rs1, uint8_t rs2) {
    setRM(rm);
    
    cpu->f[rd] = cpu->f[rs1] / cpu->f[rs2];
}

void fsqrt_handler(vCPU32 *cpu, uint8_t rd, uint8_t rm, uint8_t rs1, uint8_t func) {
    if(cpu->f[rs1] < 0.0f) exit(-1);

    cpu->f[rd] = sqrtf(cpu->f[rs1]);
}

void fmin_max_handler(vCPU32 *cpu, uint8_t rd, uint8_t func3, uint8_t rs1, uint8_t rs2) {
    //TODO
}

void feq_lt_le_handler(vCPU32 *cpu, uint8_t rd, uint8_t func3, uint8_t rs1, uint8_t rs2) {
    //TODO
}

