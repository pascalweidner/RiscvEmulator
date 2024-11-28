#include "rv32f.h"
#include "float_types.h"
#include <fenv.h>
#include <math.h>
#include <stdio.h>
#include <immintrin.h>
#include <float.h>

static inline int is_nan_gen(float32_t val) {
    return ((*(uint32_t *)&val) & 0xFF800000) == SNAN;
}

static inline int is_signaling_nan(float32_t val) {
    return ((*(uint32_t *)&val) & 0xFFC00000 ) == SNAN;
}

static inline int is_quiet_nan(float32_t val) {
    return ((*(uint32_t *)&val) & 0xFFC00000) == QNAN;
}

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
    // TODO: implement signaling nan
    if(isnanf(cpu->f[rs1]) && isnanf(cpu->f[rs2])) {
        cpu->f[rd] = NAN;
        return;
    } else if(isnanf(cpu->f[rs1])) {
        cpu->f[rd] = cpu->f[rs2];
    } else if(isnanf(cpu->f[rs2])) {
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
    if(isnanf(cpu->f[rs1]) || isnanf(cpu->f[rs2])) {
        //TODO: check for signaling NAN and set exception flag when fle or flt and only set exception flag when feq and a signaling nan is present
        cpu->f[rd] = 0;
        return;
    }

    switch(func3) {
        case 0: // FLE
            cpu->f[rd] = cpu->f[rs1] <= cpu->f[rs2];
            break;
        case 1: // FLT
            cpu->f[rd] = cpu->f[rs1] < cpu->f[rs2];
            break;
        case 2: // FEQ
            cpu->f[rd] = cpu->f[rs1] == cpu->f[rs2];
            break;
        default:
            printf("Instruction is not supported!\n");
            break;
    }
}

void fmv_fclass_handler(vCPU32 *cpu, uint8_t rd, uint8_t func3, uint8_t rs1, uint8_t plc) {
    switch(func3) {
        case 0: // FMV
            memcpy(&(cpu->f[rs1]), &(cpu->f[rd]), sizeof(float32_t));
            break;
        case 1: // FCLASS
            uint16_t result = 0;
            if(isnanf(cpu->f[rs1])) {
                //TODO: check for signaling nan
                result |= (1 << 9);
            } else if(isinff(cpu->f[rs1])) {
                result |= (signbit(cpu->f[rs1])) ? (1 << 0) : (1 << 7);
            } else if(cpu->f[rs1] == 0.0f) {
                if(signbit(cpu->f[rs1])) {
                    result |= (1 << 3);
                } else {
                    result = (1 << 4);
                }
            } else {
                if(fabsf(cpu->f[rs1]) < FLT_MIN) result |= (signbit(cpu->f[rs1])) ? (1 << 2) : (1 << 5);
                else result |= (signbit(cpu->f[rs1])) ? (1 << 1) : (1 << 6);
            }
            break;
        default:
            printf("instruction is not supported!\n");
            exit(-1);

    }
}

void fsgn_handler(vCPU32 *cpu, uint8_t rd, uint8_t func3, uint8_t rs1, uint8_t rs2) {
    switch(func3) {
        case 0: // SGNJ
            break;
        case 1: // SGNJN
            break;
        case 2: // SGNJX
            break;
        default:
            printf("instruction is not supported!\n");
            exit(-1);
    }
}