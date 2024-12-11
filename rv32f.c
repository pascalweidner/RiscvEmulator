#include "rv32f.h"
#include "bus.h"
#include "float_types.h"
#include <fenv.h>
#include <math.h>
#include <stdio.h>
#include <immintrin.h>
#include <float.h>

#define MIN_FCVTWS -2147483648
#define MAX_FCVTWS 2147483647
#define MIN_FCVTWUS 0
#define MAX_FCVTWUS 4294967295

//TODO: Handle signaling Nan as operands and other exceptions

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
                    //TODO raise illegal instruction exception
                    printf("Reserved for future use!\n");
                    exit(-1);
            }
            break;
        default:
            //TODO raise illegal instructon exception
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
    if(is_signaling_nan(cpu->f[rs1]) || is_signaling_nan(cpu->f[rs2])) {/*TODO throw exception*/}

    if(is_nan_gen(cpu->f[rs1]) && is_nan_gen(cpu->f[rs2])) {
        cpu->f[rd] = QNAN;
        return;
    } else if(is_nan_gen(cpu->f[rs1])) {
        cpu->f[rd] = cpu->f[rs2];
    } else if(is_nan_gen(cpu->f[rs2])) {
        cpu->f[rd] = cpu->f[rs1];
    }

    switch (func3) {
        case 0: // MIN
            if(cpu->f[rs1] == cpu->f[rs2]) {
                cpu->f[rd] = signbit(cpu->f[rs1]) ? cpu->f[rs1] : cpu->f[rs2];
            }
            cpu->f[rd] = (cpu->f[rs1] < cpu->f[rs2]) ? cpu->f[rs1] : cpu->f[rs2];
            break;
        case 1: // MAX
            if(cpu->f[rs1] == cpu->f[rs2]) {
                cpu->f[rd] = signbit(cpu->f[rs1]) ? cpu->f[rs2] : cpu->f[rs1];
            }
            cpu->f[rd] = (cpu->f[rs1] > cpu->f[rs2]) ? cpu->f[rs1] : cpu->f[rs2];
            break;
        default: 
            printf("Invalid func for min/max\n");
            exit(-1);
    }
}

void feq_lt_le_handler(vCPU32 *cpu, uint8_t rd, uint8_t func3, uint8_t rs1, uint8_t rs2) {
    if(is_signaling_nan(cpu->f[rs1]) || is_signaling_nan(cpu->f[rs2])) {
        /*TODO: throw exception*/
        cpu->f[rd] = 0;
        return;
    } else if(is_nan_gen(cpu->f[rs1]) || is_nan_gen(cpu->f[rs2])) {
        if(func3 == 0 || func3 == 1) {/*TODO: throw exception*/}
        cpu->f[rd] = 0;
        return;;
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
        case 0: // FMV.X.W
            memcpy(&(cpu->f[rs1]), &(cpu->x[rd]), sizeof(float32_t));
            break;
        case 1: // FCLASS
            uint32_t result = 0;
            int inf = isinff(cpu->f[rs1]);
            if(is_signaling_nan(cpu->f[rs1])) {
                result |= (1 << 8);
            } else if(is_quiet_nan(cpu->f[rs1])) {
                result |= (1 << 9);
            } else if(inf == -1) { // -inf or inf
                result |= (1 << 0);
            } else if (inf == 1) {
                result |= (1 << 7);
            } else if(cpu->f[rs1] == 0.0f) { // -0.0 or 0.0
                if(signbit(cpu->f[rs1])) {
                    result |= (1 << 3);
                } else {
                    result = (1 << 4);
                }
            } else {
                if(fabsf(cpu->f[rs1]) < FLT_MIN) result |= (signbit(cpu->f[rs1])) ? (1 << 2) : (1 << 5); // subnormal number
                else result |= (signbit(cpu->f[rs1])) ? (1 << 1) : (1 << 6); // normal number
            }
            cpu->f[rd] = result;
            break;
        default:
            printf("instruction is not supported!\n");
            exit(-1);
    }
}

void fmv_wx_handler(vCPU32 *cpu, uint8_t rd, uint8_t rm, uint8_t rs1, uint8_t plc) {
    memcpy(&(cpu->x[rs1]), &(cpu->f[rd]), sizeof(uint32_t));
}

void fsgn_handler(vCPU32 *cpu, uint8_t rd, uint8_t func3, uint8_t rs1, uint8_t rs2) {
    uint32_t res;
    switch(func3) {
        case 0: // FSGNJ
            res = (*(uint32_t *)&(cpu->f[rs1]) & 0x7FFFFFFF) | (*(uint32_t*)&(cpu->f[rs2]) & 0x80000000);
            memcpy(&(cpu->f[rd]), &res, sizeof(uint32_t));
            break;
        case 1: // FSGNJN
            res = (*(uint32_t *)&(cpu->f[rs1]) & 0x7FFFFFFF) | (~(*(uint32_t*)&(cpu->f[rs2])) & 0x80000000);
            memcpy(&(cpu->f[rd]), &res, sizeof(uint32_t));
            break;
        case 2: // FSGNJX
            res = (*(uint32_t *)&(cpu->f[rs1]) & 0x7FFFFFFF) | ((*(uint32_t*)&(cpu->f[rs2]) & 0x80000000) ^ (*(uint32_t*)&(cpu->f[rs1]) & 0x80000000));
            memcpy(&(cpu->f[rd]), &res, sizeof(uint32_t));
            break;
        default:
            printf("instruction is not supported!\n");
            exit(-1);
    }
}

void fcvt_sw_handler(vCPU32 *cpu, uint8_t rd, uint8_t rm, uint8_t rs1, uint8_t func) {
    switch(func) {
        case 0: // FCVT.S.W
            cpu->f[rd] = (float32_t)((int32_t)(cpu->x[rs1]));
            break;
        case 1: // FCVT.S.WU
            cpu->f[rd] = (float32_t)(cpu->x[rs1]);
            break;
    }
}

void fcvt_ws_handler(vCPU32 *cpu, uint8_t rd, uint8_t rm, uint8_t rs1, uint8_t func) {
    setRM(cpu, rm);
    //TODO: throw exception when nan
    float32_t res = roundf(cpu->f[rs1]);

    switch(func) {
        case 0: // FCVT.W.S
            if(res > (float32_t)MAX_FCVTWS) {
                cpu->x[rd] = MAX_FCVTWS;
                return;
            } else if(res < (float32_t)MIN_FCVTWS) {
                cpu->x[rd] = MIN_FCVTWS;
                return;
            } else if(isinff(cpu->f[rs1]) == -1 ) {
                cpu->x[rd] = MIN_FCVTWS;
                return;
            } else if(isinff(cpu->f[rs1]) == 1 || is_nan_gen(cpu->f[rs1])) {
                cpu->x[rd] = MAX_FCVTWS;
                return;
            }
            cpu->x[rd] = (int32_t)res;
            if((int32_t)(cpu->x[rd]) != (int32_t)(cpu->f[rs1])) {
                //TODO: set inexact flag
            }
            break;
        case 1: // FCVT.WU.S
            //TODO: set invalid excetpion flag
            if(res > (float32_t)MAX_FCVTWUS) {
                cpu->x[rd] = MAX_FCVTWUS;
                return;
            } else if(res < (float32_t)MIN_FCVTWUS) {
                cpu->x[rd] = MIN_FCVTWUS;
                return;
            } else if(isinff(cpu->f[rs1]) == -1) {
                cpu->x[rd] = 0;
                return;
            } else if(isinff(cpu->f[rs1]) == 1 || is_nan_gen(cpu->f[rs1])) {
                cpu->x[rd] = MAX_FCVTWUS;
                return;
            }
            cpu->x[rd] = (uint32_t)res;
            if(cpu->x[rd] != (uint32_t)(cpu->f[rs1])) {
                //TODO: set inexact flag
            }
            break;;
    }
}

void flw_handler(vCPU32 *cpu, uint16_t imm, uint8_t rd, uint8_t rs1) {
    uint32_t value = bus32_load_dram(&(cpu->bus), cpu->x[rs1] + (int32_t)imm, 32);
    memcpy(&(cpu->f[rd]), &value, sizeof(uint32_t));
}

void fsw_handler(vCPU32 *cpu, uint16_t imm, uint8_t rs1, uint8_t rs2) {
    uint32_t value;
    memcpy(&value, &(cpu->f[rs2]), sizeof(uint32_t));
    bus32_store_dram(&(cpu->bus), cpu->x[rs1] + (int32_t)imm, 32, value);
}

//TODO set invalid operation flag when rs1 and rs2 are infinity or zero in the fma instructions, even when the added is a quiet NaN
void fmadd(vCPU32 *cpu, uint32_t inst, uint8_t rd) {
    uint8_t rm = inst & 0b111;
    inst >>= 3;
    uint8_t rs1 = inst & 0b11111;
    inst >>= 5;
    uint8_t rs2 = inst & 0b11111;
    inst >>= 7;
    uint8_t rs3 = inst & 0b11111;
    setRM(cpu, rm);   

    cpu->f[rd] = fmaf(cpu->f[rs1], cpu->f[rs2], cpu->f[rs3]);
}

void fmsub(vCPU32 *cpu, uint32_t inst, uint8_t rd) {
    uint8_t rm = inst & 0b111;
    inst >>= 3;
    uint8_t rs1 = inst & 0b11111;
    inst >>= 5;
    uint8_t rs2 = inst & 0b11111;
    inst >>= 7;
    uint8_t rs3 = inst & 0b11111;
    setRM(cpu, rm);

    cpu->f[rd] = fmaf(cpu->f[rs1], cpu->f[rs2], -(cpu->f[rs3]));
}

void fnmadd(vCPU32 *cpu, uint32_t inst, uint8_t rd) {
    uint8_t rm = inst & 0b111;
    inst >>= 3;
    uint8_t rs1 = inst & 0b11111;
    inst >>= 5;
    uint8_t rs2 = inst & 0b11111;
    inst >>= 7;
    uint8_t rs3 = inst & 0b11111;
    setRM(cpu, rm);

    cpu->f[rd] = -fma(cpu->f[rs1], cpu->f[rs2], -(cpu->f[rs3]));
}

void fnmsub(vCPU32 *cpu, uint32_t inst, uint8_t rd) {
    uint8_t rm = inst & 0b111;
    inst >>= 3;
    uint8_t rs1 = inst & 0b11111;
    inst >>= 5;
    uint8_t rs2 = inst & 0b11111;
    inst >>= 7;
    uint8_t rs3 = inst & 0b11111;
    setRM(cpu, rm);

    cpu->f[rd] = -fma(cpu->f[rs1], cpu->f[rs2], cpu->f[rs3]);
}