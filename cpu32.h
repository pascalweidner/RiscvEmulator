#ifndef CPU32_H_
#define CPU32_H_
#include "bus.h"
#include "dram.h"
#include "float_types.h"

typedef struct vCPU32 vCPU32;

typedef void (*InstructionHandler)(vCPU32 *cpu, uint32_t instruction, uint8_t rd);

typedef void (*RTypeInstructionHandler)(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2);

typedef void (*FRTypeInstructionHandler)(vCPU32 *cpu, uint8_t rd, uint8_t rs1, uint8_t rs2, uint8_t rm);

typedef void (*FSTypeInstructionHandler)(vCPU32 *cpu, uint16_t imm, uint8_t rs1, uint8_t rs2);

typedef void (*FITypeInstructionHandler)(vCPU32 *cpu, uint16_t imm, uint8_t rd, uint8_t rs1);

struct vCPU32 {
    uint32_t x[32];
    float32_t f[32];
    uint32_t fcsr;
    uint32_t pc;
    BUS32 bus;

    //M-Mode registers
    // Machine Information Registers Privilege=MRO
    uint32_t mvendorid; //0xF11
    uint32_t marchid; //0xF12
    uint32_t mimpid; //0xF13
    uint32_t mhartid; //0xF14
    uint32_t mconfigptr; //0xF15

    //Machine Trap Setup Privilege=MRW
    uint32_t mstatus; // 0x300
    uint32_t misa; // 0x301
    uint32_t medeleg; // 0x302
    uint32_t mideleg; // 0x303
    uint32_t mie; // 0x304
    uint32_t mtvec; //0x305
    uint32_t mcounteren; //0x306
    uint32_t mstatush; //0x310
    uint32_t medelegh; //0x312

    //Machine Trap Handling
    uint32_t mscratch; //0x340
    uint32_t mepc; //0x341
    uint32_t mcause; //0x342
    uint32_t mtval; //0x343
    uint32_t mip; //0x344
    uint32_t mtinst; //0x34A
    uint32_t mtval2; //0x34B

    //Machine Configuration
    uint32_t menvcfg; //0x30A
    uint32_t menvcfgh; //0x31A
    uint32_t mseccfg; //0x747
    uint32_t mseccfgh; //0x757

    //Machine Memory Protection
    uint32_t pmpcfg[16]; // 0x3A0 - 0x3AF
    uint32_t pmpaddr[64]; // 0x380 - 0x3EF

    // Machine State Enable Registers
    uint32_t mstateen[4]; // 0x30C - 0x30F
    uint32_t mstateenh[4]; //0x31C - 0x31F

    // Machine Non-Maksable Interrupt Handling
    uint32_t mnscratch; // 0x740
    uint32_t mnepc; //0x741
    uint32_t mncause; //0x742
    uint32_t mnstatus; //0x744

    // Machine Counter/Timers
    uint32_t mcylce; //0xB00
    uint32_t minstret; //0xB02
    uint32_t mhpmcounter3_31[29]; //0xB03 - 0xB1F
    uint32_t mcycleh; //0xB80
    uint32_t minstreth; //0xB82
    uint32_t mhpmcounter3_31h[29]; //0xB83 - 0xB9F

    // Machine Counter Setup
    uint32_t mcountinhibit; //0x320
    uint32_t mhpmevent3_31[29]; //0x323 - 0x33F
    uint32_t mhpmevent3_31h[29]; //0x723 - 0x73F


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




#endif