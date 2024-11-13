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

void rtype(uint32_t inst, uint8_t rd)
{
    uint8_t funct3 = inst & 0b00000111;
    inst >>= 3;
}
