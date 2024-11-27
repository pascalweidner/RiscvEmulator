#include "rv32f.h"
#include <fenv.h>
#include <math.h>
#include <stdio.h>
#include <immintrin.h>

static inline void setRM(vCPU32 *cpu, uint8_t rm) {
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
        case 7:
            uint8_t frm = (cpu->fcsr >> 5) & 0b111;
            switch(frm) {
                case 0:
                    fesetround(FE_TONEAREST);
                    break;
                case 1:
                    fesetround(FE_TOWARDZERO);
                    break;
                case 2:
                    fesetround(FE_DOWNWARD);
                    break;
                default:
                    printf("Reserved for future use!\n");
                    exit(-1);
            }
            break;
        default:
            printf("Reserved for future use\n");
            exit(-1);
    }
}

void fadd_handler(vCPU32 *cpu, uint8_t rd, uint8_t rm, uint8_t rs1, uint8_t rs2) {
    setRM(cpu, rm);

    cpu->f[rd] = cpu->f[rs1] + cpu->f[rs2]; 
}

void fsub_handler(vCPU32 *cpu, uint8_t rd, uint8_t rm, uint8_t rs1, uint8_t rs2) {
    setRM(cpu, rm);

    cpu->f[rd] = cpu->f[rs1] - cpu->f[rs2];
}

void fmul_handler(vCPU32 *cpu, uint8_t rd, uint8_t rm, uint8_t rs1, uint8_t rs2) {
    setRM(cpu, rm);

    cpu->f[rd] = cpu->f[rs1] - cpu->f[rs2];
}

void fdiv_handler(vCPU32 *cpu, uint8_t rd, uint8_t rm, uint8_t rs1, uint8_t rs2) {
    setRM(cpu, rm);
    
    cpu->f[rd] = cpu->f[rs1] / cpu->f[rs2];
}

void fsqrt_handler(vCPU32 *cpu, uint8_t rd, uint8_t rm, uint8_t rs1, uint8_t func) {
    if(cpu->f[rs1] < 0.0f) {
        cpu->f[rd] = NAN;
        return;
    }
    setRM(cpu, rm);

    cpu->f[rd] = sqrtf(cpu->f[rs1]);
}

void fmin_max_handler(vCPU32 *cpu, uint8_t rd, uint8_t func3, uint8_t rs1, uint8_t rs2) {
    if(isnan(cpu->f[rs1]) && isnan(cpu->f[rs2])) {
        cpu->f[rd] = NAN;
        return;
    } else if(isnan(cpu->f[rs1])) {
        cpu->f[rd] = cpu->f[rs2];
    } else if(isnan(cpu->f[rs2])) {
        cpu->f[rd] = cpu->f[rs1];
    }

    switch (func3) {
        case 0: // MIN
            cpu->f[rd] = (cpu->f[rs1] < cpu->f[rs2]) ? rs1 : rs2;
            break;
        case 1: // MAX
            cpu->f[rd] = (cpu->f[rs1] > cpu->f[rs2]) ? rs1 : rs2;
            break;
        default: 
            printf("Invalid func for min/max\n");
            exit(-1);
    }
}

void feq_lt_le_handler(vCPU32 *cpu, uint8_t rd, uint8_t func3, uint8_t rs1, uint8_t rs2) {
    //TODO
}

