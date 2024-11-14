#include "cpu.h"

// R-type Instructions
#define RTYPE 0110011
#define STYPE 0100011
#define ITYPE 0000011
#define ITYPE2 0010011
#define ITYPE3 1100111
#define JTYPE 1101111
#define UTYPE 0010111
#define BTYPE 1100011
 
vCPU32 *init_vCPU32(DRAM32 *dram) {
    vCPU32 *cpu = (vCPU32*)malloc(sizeof(vCPU32));
    cpu->pc = DRAM32_BASE;
    cpu->x[0] = 0;
    init_bus32(cpu->bus, dram);
}

void cpu32_run(vCPU32 *cpu) {

}

static void rtype(uint32_t inst, uint8_t rd);
static void stype(uint32_t inst, uint8_t rd);
static void btype(uint32_t inst, uint8_t rd);
static void utype(uint32_t inst, uint8_t rd);
static void itype(uint32_t inst, uint8_t rd);
static void itype2(uint32_t inst, uint8_t rd);
static void itype3(uint32_t inst, uint8_t rd);
static void jtype(uint32_t inst, uint8_t rd);

static int cpu32_execute(vCPU32 *cpu) {
    uint32_t inst = bus32_load_dram(&cpu->bus, cpu->pc, 32);

    uint8_t op = inst & 0b01111111;
    inst >>= 7;
    uint8_t rd = inst & 0b00011111;
    inst >>= 5;
    switch (op)
    {
        case RTYPE: 
            rtype(inst, rd);
            break;
        case STYPE:
            stype(inst, rd);
            break;
        case BTYPE:
            btype(inst, rd);
            break;
        case UTYPE:
            utype(inst, rd);
            break;
        case ITYPE:
            itype(inst, rd);
            break;
        case ITYPE2:
            itype2(inst, rd);
            break;
        case ITYPE3:
            itype3(inst, rd);
            break;
        case JTYPE:
            jtype(inst, rd);
            break;
        default:
            return -1;
            break;
    }
}

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

void rtype(uint32_t inst, uint8_t rd)
{
    uint16_t funct = inst & 0b00000111;
    inst >>= 3;
    uint8_t rs1 = inst &0b00011111;
    inst >>= 5;
    uint8_t rs2 = inst & 0b00011111;
    inst >>= 5;
    funct = (funct << 7) | inst;

    switch(funct) {
        case ADD_INST:
            //TODO: implement add
            break;
        case SUB_INST:
            //TODO: implement sub
            break;
        case SLL_INST:
            //TODO: implement shift left logical
            break;
        case SLT_INST:
            //TODO: implement set less than
            break;
        case SLTU_INST:
            //TODO: implement set less than unsigned
            break;
        case XOR_INST:
            //TODO: implemnt xor
            break;
        case SRL_INST:
            //TODO: implement shift right logical
            break;
        case OR_INST:
            //TODO: implement or
            break;
        case AND_INST:
            //TODO: implemnt and
            break;
        case 
    }

}

void stype(uint32_t inst, uint8_t rd)
{
    //TODO:
}

#define LB_INST 0b000
#define LH_INST 0b001
#define LW_INST 0b010
#define LBU_INST 0b100 
#define LHU_INST 0b101

void itype(uint32_t inst, uint8_t rd)
{
    uint8_t funct3 = inst & 0b00000111;
    inst >>= 3;
    uint8_t rs1 = inst & 0b00011111;
    inst >>= 5;

    switch(funct3) {
        case LB_INST:
            //TODO: implement load byte instruction
            break;
        case LH_INST:
            //TODO: implement load half word instruction
            break;
        case LW_INST:
            //TODO: implement load word instruction
            break;
        case LBU_INST:
            //TODO: implement load byte unsigned
            break;
        case LHU_INST:
            //TODO: implement load half word unsigned
            break;
    }
}