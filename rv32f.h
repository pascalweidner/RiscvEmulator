#ifndef RV32F_H_
#define RV32F_H_
#include "cpu32.h"
#include "includes.h"
#include <stdint.h>

#define FITYPE 0b0000111
#define FRTYPE 0b1010011
#define FMADD 0b1000011
#define FMSUB 0b1000111
#define FNMSUB 0b1001011
#define FNMADD 0b1001111
#define FSTYPE 0b0100111

#define FADD_INST 0b0
#define FSUB_INST 0b100
#define FMUL_INST 0b1000
#define FDIV_INST 0b1100
#define FSQRT_INST 0b101100
#define FSGN_INST 0b10000 // implement switch internally (FSGNJ, FSGNJX, FSGNJN)
#define FMIN_MAX_INST 0b10100
#define FCVT_WS_INST 0b1100000 // internally a switch statement (FCVT.W.S, FCVT.WU.S)
#define FMV_FCLASS_INST 0b1110000
#define FEQ_FLT_FLE_INST 0b1010000
#define FCVT_SW_INST 0b1101000
#define FMV_WX_INST 0b1111000

#define NV_FMnemonic 0b10000 // Invalid Operation flag
#define DZ_FMnemonic 0b1000 // Divide by Zero flag
#define OF_FMnemonic 0b100 // Overflow flag
#define UF_FMnemonic 0b10 // Underflow flag
#define NX_FMnemonic 0b1 // Inexact flag


#define QNAN 0x7FC00000
#define SNAN 0x7F800000

void fadd_handler(vCPU32 *cpu, uint8_t rd, uint8_t rm, uint8_t rs1, uint8_t rs2);
void fsub_handler(vCPU32 *cpu, uint8_t rd, uint8_t rm, uint8_t rs1, uint8_t rs2);
void fmul_handler(vCPU32 *cpu, uint8_t rd, uint8_t rm, uint8_t rs1, uint8_t rs2);
void fdiv_handler(vCPU32 *cpu, uint8_t rd, uint8_t rm, uint8_t rs1, uint8_t rs2);
void fsqrt_handler(vCPU32 *cpu, uint8_t rd, uint8_t rm, uint8_t rs1, uint8_t func);
void fmin_max_handler(vCPU32 *cpu, uint8_t rd, uint8_t func3, uint8_t rs1, uint8_t rs2);
void feq_lt_le_handler(vCPU32 *cpu, uint8_t rd, uint8_t func3, uint8_t rs1, uint8_t rs2);
void fmv_fclass_handler(vCPU32 *cpu, uint8_t rd, uint8_t func3, uint8_t rs1, uint8_t plc);
void fsgn_handler(vCPU32 *cpu, uint8_t rd, uint8_t func3, uint8_t rs1, uint8_t rs2);
void fcvt_ws_handler(vCPU32 *cpu, uint8_t rd, uint8_t rm, uint8_t rs1, uint8_t func);
void fcvt_sw_handler(vCPU32 *cpu, uint8_t rd, uint8_t rm, uint8_t rs1, uint8_t func);
void fmv_wx_handler(vCPU32 *cpu, uint8_t rd, uint8_t rm, uint8_t rs1, uint8_t plc);



#endif